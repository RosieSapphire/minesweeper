#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "tile.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

static GLuint       tile_shader;
static GLuint       tile_tex;
static GLuint       tile_bomb_tex;
static GLuint       tile_flagged_tex;
static GLuint       tile_empty_tex[9];
static unsigned int vao, vbo, ebo;

static const int indis[6] = { 0, 1, 2, 2, 1, 3 };

struct tile {
        float    col[3];
        uint32_t flags;
};

static bool is_game_lost = false;

static struct tile tiles[TILES_X][TILES_Y];

void tiles_init(void)
{
        tile_shader = shader_load("res/vert.glsl", "res/frag.glsl");

        tile_tex         = texture_load("res/tile.png");
        tile_bomb_tex    = texture_load("res/tile_bomb.png");
        tile_flagged_tex = texture_load("res/tile_flagged.png");

        for (int i = 0; i < 9; i++) {
                char buf[128];
                sprintf(buf, "res/tile_empty_%d.png", i);
                tile_empty_tex[i] = texture_load(buf);
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(float) * 4,
                              NULL);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(indis),
                     indis,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        srand((uint32_t)time(NULL));

        for (uint16_t y = 0; y < TILES_Y; y++) {
                for (uint16_t x = 0; x < TILES_X; x++) {
                        static const float r255 = 1.0f / 255.0f;
                        struct tile *const cur  = &tiles[x][y];

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
        const uint16_t x_start = (uint16_t)max(0, tx - 1);
        const uint16_t y_start = (uint16_t)max(0, ty - 1);
        const uint16_t x_end   = (uint16_t)min(TILES_X, tx + 2);
        const uint16_t y_end   = (uint16_t)min(TILES_Y, ty + 2);

        uint32_t num = 0u;

        for (int y = y_start; y < y_end; y++)
                for (int x = x_start; x < x_end; x++)
                        num += (tiles[x][y].flags & TILE_FLAG_HAS_BOMB) >>
                               TILE_FLAG_HAS_BOMB_SHIFT;

        return num;
}

static void mouse_pos_get_as_tile(int16_t *const o,
                                  const uint16_t win_wid,
                                  const uint16_t win_hei)
{
        window_mouse_pos_get(o);
        if (o[0] < 0 || o[0] >= win_wid || o[1] < 0 || o[0] >= win_hei)
                return;

        o[0] /= TILE_SIZE;
        o[1] = win_hei - (uint16_t)o[1];
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

        x_start = (uint16_t)max(0, tx - 1);
        y_start = (uint16_t)max(0, ty - 1);
        x_end   = (uint16_t)min(TILES_X, tx + 2);
        y_end   = (uint16_t)min(TILES_Y, ty + 2);

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

void tiles_update(const uint16_t win_wid, const uint16_t win_hei)
{
        int16_t mouse[2];

        if (is_game_lost)
                return;

        if (is_game_won())
                printf("YOU'RE WINNER!\n");

        mouse_pos_get_as_tile(mouse, win_wid, win_hei);

        if (window_lmb_held()) {
                tile_reveal((uint16_t)mouse[0], (uint16_t)mouse[1]);
                return;
        }

        if (window_rmb_held()) {
                tiles[mouse[0]][mouse[1]].flags ^= TILE_FLAG_IS_FLAGGED;
                return;
        }
}

static void tile_draw(const uint16_t tx,
                      const uint16_t ty,
                      const uint16_t win_width,
                      const uint16_t win_height)
{
        const uint32_t surround_cnt =
                tile_get_surrounding_bombs_count(tx, ty);
        const uint16_t           tile_x = tx * TILE_SIZE;
        const uint16_t           tile_y = ty * TILE_SIZE;
        const struct tile *const t      = &tiles[tx][ty];

        const float rect[4] = {
                (float)tile_x / (float)win_width,
                (float)tile_y / (float)win_height,
                (float)TILE_SIZE / (float)win_width,
                (float)TILE_SIZE / (float)win_height,
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

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUseProgram(tile_shader);

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
                       sizeof(indis) / sizeof(*indis),
                       GL_UNSIGNED_INT,
                       indis);
        glBindVertexArray(0);
}

void tiles_draw(const uint16_t win_wid, const uint16_t win_hei)
{
        for (uint16_t y = 0; y < TILES_Y; y++)
                for (uint16_t x = 0; x < TILES_X; x++)
                        tile_draw(x, y, win_wid, win_hei);
}

void tiles_terminate(void)
{
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);

        textures_unload(9, tile_empty_tex);
        textures_unload(1, &tile_flagged_tex);
        textures_unload(1, &tile_bomb_tex);
        textures_unload(1, &tile_tex);
        shader_unload(tile_shader);
}
