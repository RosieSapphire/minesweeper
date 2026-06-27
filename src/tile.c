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

enum game_state {
        GS_PLAYING = 0,
        GS_WON,
        GS_LOST,
};

struct quad {
        uint32_t *idx_arr;
        uint32_t  idx_cnt;
        uint32_t  vao;
        uint32_t  vbo;
        uint32_t  ebo;
};

static uint32_t tile_tex          = 0u;
static uint32_t tile_bomb_tex     = 0u;
static uint32_t tile_flagged_tex  = 0u;
static uint32_t tile_empty_tex[9] = {
        0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
};
static struct quad tile_quad = {
        .idx_arr = NULL,
        .idx_cnt = 0u,
        .vao     = 0u,
        .vbo     = 0u,
        .ebo     = 0u,
};

struct tile {
        float    col[3];
        uint32_t flags;
};

static enum game_state game_state = GS_PLAYING;

static struct tile tiles[TILES_X * TILES_Y];

static void tiles_textures_load(void)
{
        tile_tex         = texture_load("res/tile.png");
        tile_bomb_tex    = texture_load("res/tile_bomb.png");
        tile_flagged_tex = texture_load("res/tile_flagged.png");

        for (uint8_t i = 0; i < 9u; i++) {
                char buf[128];
                sprintf(buf, "res/tile_empty_%u.png", i);
                tile_empty_tex[i] = texture_load(buf);
        }
}

static void tiles_quad_gen(struct quad *const restrict quad,
                           const uint32_t *const restrict idx_arr,
                           const uint32_t idx_cnt)
{
        /* Ensure sane input. */
        assertf(quad, "Model pointer is NULL.");
        assertf(!quad->idx_arr,
                "Index Array already allocated: <%p>.",
                quad->idx_arr);
        assertf(!quad->idx_cnt, "Index Count is already %lu.", quad->idx_cnt);
        assertf(!quad->vao, "VAO already initialized: %u.", quad->vao);
        assertf(!quad->vbo, "VBO already initialized: %u.", quad->vbo);
        assertf(!quad->ebo, "EBO already initialized: %u.", quad->ebo);

        assertf(idx_arr, "Input index array is NULL.");
        assertf(idx_cnt, "Input index count is 0.");

        /* Allocate buffers. */
        glGenVertexArrays(1, &quad->vao);
        assertf(quad->vao, "Failed to generate VAO.");
        glBindVertexArray(quad->vao);

        glGenBuffers(1, &quad->vbo);
        assertf(quad->vbo, "Failed to generate VBO.");
        glBindBuffer(GL_ARRAY_BUFFER, quad->vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(float) * 4,
                              NULL);

        glGenBuffers(1, &quad->ebo);
        assertf(quad->ebo, "Failed to generate EBO.");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(*idx_arr) * idx_cnt,
                     idx_arr,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        /* Store the index buffer. */
        quad->idx_cnt = idx_cnt;
        quad->idx_arr = (uint32_t *)malloc(sizeof(*idx_arr) * idx_cnt);
        assertf(quad->idx_arr,
                "Failed to allocate stored index "
                "array of count %lu and size %lu.",
                idx_cnt,
                idx_cnt * sizeof(*idx_arr));
        (void)memcpy(quad->idx_arr, idx_arr, idx_cnt * sizeof(*idx_arr));
}

static void tiles_reset_board(void)
{
        game_state = GS_PLAYING;
        for (uint16_t i = 0u; i < TILES_X * TILES_Y; i++) {
                static const float r255 = 1.0f / 255.0f;
                struct tile *const cur  = &tiles[i];

                /* FIXME: What the fuck does this even do?! */
                for (uint8_t j = 0u; j < 3u; ++j)
                        cur->col[j] = (float)((uint8_t)rand()) * r255;

                cur->flags = TILES_NONE;
                if (!(rand() & 0x7))
                        cur->flags |= TILE_HAS_BOMB;
        }
}

void tiles_init(void)
{
        tiles_textures_load();
        tiles_quad_gen(&tile_quad,
                       (const uint32_t[6u]) { 0u, 1u, 2u, 2u, 1u, 3u },
                       6u);

        srand((uint32_t)time(NULL));

        tiles_reset_board();
}

static uint32_t tile_get_surrounding_bombs_count(const uint16_t tx,
                                                 const uint16_t ty)
{
        const uint16_t x_start = (uint16_t)MAX(0, tx - 1);
        const uint16_t y_start = (uint16_t)MAX(0, ty - 1);
        const uint16_t x_end   = (uint16_t)MIN(TILES_X, tx + 2);
        const uint16_t y_end   = (uint16_t)MIN(TILES_Y, ty + 2);

        uint32_t num = 0u;

        for (uint16_t y = y_start; y < y_end; y++)
                for (uint16_t x = x_start; x < x_end; x++)
                        num += (tiles[y * TILES_X + x].flags &
                                TILE_HAS_BOMB) >>
                               TILE_HAS_BOMB_SHIFT;

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

static inline uint32_t tiles_get_revealed_count(void)
{
        uint32_t cnt = 0u;

        for (uint16_t i = 0u; i < TILES_X * TILES_Y; i++)
                if (tiles[i].flags & TILE_IS_REVEALED)
                        ++cnt;

        return cnt;
}

static void tile_reveal(const uint16_t tx, const uint16_t ty)
{
        struct tile *const t = &tiles[ty * TILES_X + tx];
        uint32_t           reveal_before;
        uint16_t           x_start, y_start, x_end, y_end;

        if ((t->flags & TILE_IS_REVEALED) || (t->flags & TILE_IS_FLAGGED))
                return;

        reveal_before = tiles_get_revealed_count();
        t->flags |= TILE_IS_REVEALED;

        if (t->flags & TILE_HAS_BOMB) {
                if (__builtin_expect(reveal_before, true)) {
                        game_state = GS_LOST;
                        return;
                }

                /* Special case for if our first pick happens to be a bomb. */
                do {
                        tiles_reset_board();
                } while (t->flags & TILE_HAS_BOMB);

                t->flags |= TILE_IS_REVEALED;
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

static bool game_check_won(void)
{
        uint32_t bomb_cnt   = 0u;
        uint32_t hidden_cnt = 0u;

        for (uint16_t i = 0u; i < TILES_X * TILES_Y; i++) {
                const uint32_t f = tiles[i].flags;

                bomb_cnt += (f & TILE_HAS_BOMB) >> TILE_HAS_BOMB_SHIFT;
                hidden_cnt += !(f & TILE_IS_REVEALED);
        }

        return (bomb_cnt == hidden_cnt);
}

static inline void tiles_remaining_flag(void)
{
        for (uint16_t i = 0u; i < TILES_X * TILES_Y; i++)
                if (!(tiles[i].flags & TILE_IS_REVEALED))
                        tiles[i].flags |= TILE_IS_FLAGGED;
}

void tiles_update(const struct window *const wnd, const struct input inp)
{
#define PRINT_NONE (0u)
#define PRINT_WON  (1u << 0u)
#define PRINT_LOST (1u << 1u)

        static uint8_t printed = PRINT_NONE;

        int16_t mouse[2];

        if (inp.flags & INPUT_R_PRESS) {
                printed = PRINT_NONE;
                tiles_reset_board();
                return;
        }

        if (game_state == GS_LOST) {
                if (printed & PRINT_LOST)
                        return;

                printf("YOU LOST! Press 'R' to try again.\n");
                printed |= PRINT_LOST;
                return;
        } else if (game_state == GS_WON) {
                if (printed & PRINT_WON)
                        return;

                printf("YOU'RE WINNER! Press 'R' to play again! :D\n");
                printed |= PRINT_WON;
                return;
        }

        mouse_pos_get_as_tile(wnd, mouse);

        if (inp.flags & INPUT_LMB_PRESS) {
                const uint16_t x = (uint16_t)mouse[0];
                const uint16_t y = (uint16_t)mouse[1];

                tile_reveal(x, y);
                if (game_check_won()) {
                        tiles_remaining_flag();
                        game_state = GS_WON;
                }

                return;
        }

        if (inp.flags & INPUT_RMB_PRESS) {
                const uint32_t i =
                        (uint32_t)mouse[1] * TILES_X + (uint32_t)mouse[0];

                tiles[i].flags ^= TILE_IS_FLAGGED;
                return;
        }

#undef PRINT_LOST
#undef PRINT_WON
#undef PRINT_NONE
}

static void tile_draw(const uint32_t shd,
                      const uint32_t wnd_wid,
                      const uint32_t wnd_hei,
                      const uint16_t tx,
                      const uint16_t ty)
{
        const uint32_t surround_cnt =
                tile_get_surrounding_bombs_count(tx, ty);
        const uint16_t           tile_x = tx * TILE_SIZE;
        const uint16_t           tile_y = ty * TILE_SIZE;
        const struct tile *const t      = &tiles[ty * TILES_X + tx];

        const float rect[4] = {
                (float)tile_x / (float)wnd_wid,
                (float)tile_y / (float)wnd_hei,
                (float)TILE_SIZE / (float)wnd_wid,
                (float)TILE_SIZE / (float)wnd_hei,
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
        glBindVertexArray(tile_quad.vao);
        glBindBuffer(GL_ARRAY_BUFFER, tile_quad.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUseProgram(shd);

        glBindTexture(GL_TEXTURE_2D, tile_tex);

        if (t->flags & TILE_IS_FLAGGED)
                glBindTexture(GL_TEXTURE_2D, tile_flagged_tex);

        if (t->flags & TILE_IS_REVEALED)
                glBindTexture(GL_TEXTURE_2D, tile_empty_tex[surround_cnt]);

        if (t->flags & TILE_HAS_BOMB) {
                if (t->flags & TILE_IS_REVEALED ||
                    (game_state == GS_LOST && !(t->flags & TILE_IS_REVEALED)))
                        glBindTexture(GL_TEXTURE_2D, tile_bomb_tex);
        }

        glDrawElements(GL_TRIANGLES,
                       (GLsizei)tile_quad.idx_cnt,
                       GL_UNSIGNED_INT,
                       tile_quad.idx_arr);
        glBindVertexArray(0);
}

void tiles_draw(const uint32_t shd,
                const uint32_t wnd_wid,
                const uint32_t wnd_hei)
{
        for (uint16_t i = 0u; i < TILES_X * TILES_Y; i++)
                tile_draw(shd, wnd_wid, wnd_hei, x, y);
}

static void tiles_terminate_quad(struct quad *const q)
{
        glDeleteBuffers(1, &q->ebo);
        q->ebo = 0u;
        glDeleteBuffers(1, &q->vbo);
        q->vbo = 0u;
        glDeleteVertexArrays(1, &q->vao);
        q->vao     = 0u;
        q->idx_cnt = 0u;
        free(q->idx_arr);
        q->idx_arr = NULL;
}

void tiles_terminate(void)
{
        tiles_terminate_quad(&tile_quad);

        for (uint8_t i = 0u; i < 9u; ++i)
                texture_unload(&tile_empty_tex[i]);

        texture_unload(&tile_flagged_tex);
        texture_unload(&tile_bomb_tex);
        texture_unload(&tile_tex);
}
