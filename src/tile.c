#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/param.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "error.h"

#include "tile.h"

/* TODO: Move somewhere else maybe? */
struct model {
        uint32_t *idx_arr;
        uint32_t  idx_cnt;
        uint32_t  vao;
        uint32_t  vbo;
        uint32_t  ebo;
};

static uint32_t tile_tex          = 0u;
static uint32_t tile_bomb_tex     = 0u;
static uint32_t tile_flagged_tex  = 0u;
static uint32_t tile_empty_tex[9] = { 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };
static struct model tile_model    = { .vao = 0u, .vbo = 0u, .ebo = 0u };

struct tile {
        float    col[3];
        uint32_t flags;
};

static bool is_game_lost = false;

static struct tile tiles[TILES_X][TILES_Y];

static void tiles_load_textures(struct renderer *const rnd)
{
        tile_tex         = renderer_texture_load(rnd, "res/tile.png");
        tile_bomb_tex    = renderer_texture_load(rnd, "res/tile_bomb.png");
        tile_flagged_tex = renderer_texture_load(rnd, "res/tile_flagged.png");

        for (uint8_t i = 0; i < 9u; i++) {
                char buf[128];
                sprintf(buf, "res/tile_empty_%u.png", i);
                tile_empty_tex[i] = renderer_texture_load(rnd, buf);
        }
}

static void tiles_build_model(struct model *const restrict mdl,
                              const uint32_t *const restrict idx_arr,
                              const uint32_t idx_cnt)
{
        /* Ensure sane input. */
        assertf(mdl, "Model pointer is NULL.");
        assertf(!mdl->idx_arr,
                "Index Array already allocated: <%p>.",
                mdl->idx_arr);
        assertf(!mdl->idx_cnt, "Index Count is already %lu.", mdl->idx_cnt);
        assertf(!mdl->vao, "VAO already initialized: %u.", mdl->vao);
        assertf(!mdl->vbo, "VBO already initialized: %u.", mdl->vbo);
        assertf(!mdl->ebo, "EBO already initialized: %u.", mdl->ebo);

        assertf(idx_arr, "Input index array is NULL.");
        assertf(idx_cnt, "Input index count is 0.");

        /* Allocate buffers. */
        glGenVertexArrays(1, &mdl->vao);
        assertf(mdl->vao, "Failed to generate VAO.");
        glBindVertexArray(mdl->vao);

        glGenBuffers(1, &mdl->vbo);
        assertf(mdl->vbo, "Failed to generate VBO.");
        glBindBuffer(GL_ARRAY_BUFFER, mdl->vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(float) * 4,
                              NULL);

        glGenBuffers(1, &mdl->ebo);
        assertf(mdl->ebo, "Failed to generate EBO.");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mdl->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(*idx_arr) * idx_cnt,
                     idx_arr,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        /* Store the index buffer. */
        mdl->idx_cnt = idx_cnt;
        mdl->idx_arr = (uint32_t *)malloc(sizeof(*idx_arr) * idx_cnt);
        assertf(mdl->idx_arr,
                "Failed to allocate stored index "
                "array of count %lu and size %lu.",
                idx_cnt,
                idx_cnt * sizeof(*idx_arr));
        (void)memcpy(mdl->idx_arr, idx_arr, idx_cnt * sizeof(*idx_arr));
}

void tiles_init(struct renderer *const rnd)
{
        tiles_load_textures(rnd);
        tiles_build_model(&tile_model,
                          (const uint32_t[6u]) { 0u, 1u, 2u, 2u, 1u, 3u },
                          6u);

        srand((uint32_t)time(NULL));

        for (uint16_t y = 0; y < TILES_Y; y++) {
                for (uint16_t x = 0; x < TILES_X; x++) {
                        static const float r255 = 1.0f / 255.0f;
                        struct tile *const cur  = &tiles[x][y];

                        /* FIXME: What the fuck does this even do?! */
                        for (uint8_t i = 0u; i < 3u; ++i)
                                cur->col[i] = (float)((uint8_t)rand()) * r255;

                        cur->flags = TILE_FLAGS_NONE;
                        if (!(rand() & 0x7))
                                cur->flags |= TILE_FLAG_HAS_BOMB;
                }
        }
}

static uint32_t tile_get_surrounding_bombs_count(const uint16_t tx,
                                                 const uint16_t ty)
{
        const uint16_t x_start = (uint16_t)MAX(0, tx - 1);
        const uint16_t y_start = (uint16_t)MAX(0, ty - 1);
        const uint16_t x_end   = (uint16_t)MIN(TILES_X, tx + 2);
        const uint16_t y_end   = (uint16_t)MIN(TILES_Y, ty + 2);

        uint32_t num = 0u;

        for (int y = y_start; y < y_end; y++)
                for (int x = x_start; x < x_end; x++)
                        num += (tiles[x][y].flags & TILE_FLAG_HAS_BOMB) >>
                               TILE_FLAG_HAS_BOMB_SHIFT;

        return num;
}

static void mouse_pos_get_as_tile(const struct window *const restrict wnd,
                                  int16_t *const restrict o)
{
        window_mouse_pos_get(wnd, o);
        if (o[0] < 0 || (uint32_t)o[0] >= wnd->width || o[1] < 0 ||
            (uint32_t)o[0] >= wnd->height)
                return;

        o[0] /= TILE_SIZE;
        o[1] = (int16_t)wnd->height - o[1];
        o[1] /= TILE_SIZE;
}

static void tile_reveal(const uint16_t tx, const uint16_t ty)
{
        struct tile *const t = &tiles[tx][ty];
        uint16_t           x_start, y_start, x_end, y_end;

        if ((t->flags & TILE_FLAG_IS_REVEALED) ||
            (t->flags & TILE_FLAG_IS_FLAGGED))
                return;

        t->flags |= TILE_FLAG_IS_REVEALED;

        if (t->flags & TILE_FLAG_HAS_BOMB) {
                is_game_lost = true;
                return;
        }

        if (tile_get_surrounding_bombs_count(tx, ty))
                return;

        x_start = (uint16_t)MAX(0, tx - 1);
        y_start = (uint16_t)MAX(0, ty - 1);
        x_end   = (uint16_t)MIN(TILES_X, tx + 2);
        y_end   = (uint16_t)MIN(TILES_Y, ty + 2);

        for (uint16_t y = y_start; y < y_end; y++)
                for (uint16_t x = x_start; x < x_end; x++)
                        tile_reveal(x, y);
}

static bool is_game_won(void)
{
        uint32_t bomb_cnt   = 0u;
        uint32_t hidden_cnt = 0u;

        for (uint16_t y = 0; y < TILES_Y; y++) {
                for (uint16_t x = 0; x < TILES_X; x++) {
                        const uint32_t f = tiles[x][y].flags;

                        bomb_cnt += (f & TILE_FLAG_HAS_BOMB) >>
                                    TILE_FLAG_HAS_BOMB_SHIFT;
                        hidden_cnt += !(f & TILE_FLAG_IS_REVEALED);
                }
        }

        return (bomb_cnt == hidden_cnt);
}

void tiles_update(const struct window *const wnd, const struct input inp)
{
        static bool printed_message = false;

        int16_t mouse[2];

        if (is_game_lost)
                return;

        if (is_game_won() && !printed_message) {
                printf("YOU'RE WINNER!\n");
                printed_message = true;
                return;
        }

        mouse_pos_get_as_tile(wnd, mouse);

        if (inp.flags & INPUT_LMB_PRESS) {
                tile_reveal((uint16_t)mouse[0], (uint16_t)mouse[1]);
                return;
        }

        if (inp.flags & INPUT_RMB_PRESS) {
                tiles[mouse[0]][mouse[1]].flags ^= TILE_FLAG_IS_FLAGGED;
                return;
        }
}

static void tile_draw(const struct renderer *const restrict rnd,
                      const struct window *const restrict wnd,
                      const uint16_t tx,
                      const uint16_t ty)
{
        const uint32_t surround_cnt =
                tile_get_surrounding_bombs_count(tx, ty);
        const uint16_t           tile_x = tx * TILE_SIZE;
        const uint16_t           tile_y = ty * TILE_SIZE;
        const struct tile *const t      = &tiles[tx][ty];

        const float rect[4] = {
                (float)tile_x / (float)wnd->width,
                (float)tile_y / (float)wnd->height,
                (float)TILE_SIZE / (float)wnd->width,
                (float)TILE_SIZE / (float)wnd->height,
        };

        /* FIXME: This is a pretty fucken cringe way to do this. qnq */
        float verts[4][4] = {
                {           rect[0],           rect[1], 0, 1 },
                { rect[0] + rect[2],           rect[1], 1, 1 },
                {           rect[0], rect[1] + rect[3], 0, 0 },
                { rect[0] + rect[2], rect[1] + rect[3], 1, 0 },
        };

        /* Normalizing the vertices to OpenGL's coordinate system. */
        for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 4; j++) {
                        float *const cur = &verts[j][i];

                        *cur *= 2;
                        *cur -= 1;
                }
        }

        /* FIXME: use matrices! */
        glBindVertexArray(tile_model.vao);
        glBindBuffer(GL_ARRAY_BUFFER, tile_model.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUseProgram(rnd->shader);

        glBindTexture(GL_TEXTURE_2D, tile_tex);

        if (t->flags & TILE_FLAG_IS_FLAGGED)
                glBindTexture(GL_TEXTURE_2D, tile_flagged_tex);

        if (t->flags & TILE_FLAG_IS_REVEALED)
                glBindTexture(GL_TEXTURE_2D, tile_empty_tex[surround_cnt]);

        if (t->flags & TILE_FLAG_HAS_BOMB) {
                if (t->flags & TILE_FLAG_IS_REVEALED ||
                    (is_game_lost && !(t->flags & TILE_FLAG_IS_REVEALED)))
                        glBindTexture(GL_TEXTURE_2D, tile_bomb_tex);
        }

        glDrawElements(GL_TRIANGLES,
                       (GLsizei)tile_model.idx_cnt,
                       GL_UNSIGNED_INT,
                       tile_model.idx_arr);
        glBindVertexArray(0);
}

void tiles_draw(const struct renderer *const restrict rnd,
                const struct window *const restrict wnd)
{
        for (uint16_t y = 0; y < TILES_Y; y++)
                for (uint16_t x = 0; x < TILES_X; x++)
                        tile_draw(rnd, wnd, x, y);
}

void tiles_terminate(struct renderer *const rnd)
{
        glDeleteBuffers(1, &tile_model.ebo);
        tile_model.ebo = 0u;
        glDeleteBuffers(1, &tile_model.vbo);
        tile_model.vbo = 0u;
        glDeleteVertexArrays(1, &tile_model.vao);
        tile_model.vao     = 0u;
        tile_model.idx_cnt = 0u;
        free(tile_model.idx_arr);
        tile_model.idx_arr = NULL;

        for (uint8_t i = 0u; i < 9u; ++i) {
                renderer_texture_unload(rnd, tile_empty_tex[i]);
                tile_empty_tex[i] = 0u;
        }

        renderer_texture_unload(rnd, tile_flagged_tex);
        tile_flagged_tex = 0u;
        renderer_texture_unload(rnd, tile_bomb_tex);
        tile_bomb_tex = 0u;
        renderer_texture_unload(rnd, tile_tex);
        tile_tex = 0u;
}
