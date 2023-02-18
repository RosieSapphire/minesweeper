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

static GLuint tile_shader;
static GLuint tile_tex;
static GLuint tile_bomb_tex;
static GLuint tile_flagged_tex;
static GLuint tile_empty_tex[9];
static unsigned int vao, vbo, ebo;

static const int indis[6] = {
	0, 1, 2,
	2, 1, 3
};

struct tile {
	bool has_bomb;
	bool is_flagged;
	bool is_revealed;
	float col[3];
};

static bool is_game_lost = 0;

static struct tile tiles[TILES_X][TILES_Y];

void tiles_init(void)
{
	tile_shader = shader_load("res/vert.glsl", "res/frag.glsl");

	tile_tex = texture_load("res/tile.png");
	tile_bomb_tex = texture_load("res/tile_bomb.png");
	tile_flagged_tex = texture_load("res/tile_flagged.png");

	for(int i = 0; i < 9; i++) {
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4, NULL);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(indis), indis, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	srand(time(NULL));

	for(int y = 0; y < TILES_Y; y++) {
		for(int x = 0; x < TILES_X; x++) {
			struct tile *cur = &tiles[x][y];
			cur->col[0] = (float)((uint8_t)rand()) / 255.0f;
			cur->col[1] = (float)((uint8_t)rand()) / 255.0f;
			cur->col[2] = (float)((uint8_t)rand()) / 255.0f;
			cur->has_bomb = !(rand() % 8);
			cur->is_flagged = 0;
			cur->is_revealed = 0;
		}
	}
}

static int tile_get_surrounding_bombs_count(int tx, int ty)
{
	int x_start = max(0, tx - 1);
	int y_start = max(0, ty - 1);
	int x_end = min(TILES_X, tx + 2);
	int y_end = min(TILES_Y, ty + 2);

	int num = 0;

	for(int y = y_start; y < y_end; y++) {
		for(int x = x_start; x < x_end; x++) {
			num += tiles[x][y].has_bomb;
		}
	}

	return num;
}

static void mouse_pos_get_as_tile(int *out, int window_height)
{
	window_mouse_pos_get(out);
	out[0] /= TILE_SIZE;
	out[1] = window_height - out[1];
	out[1] /= TILE_SIZE;
}

static void tile_reveal(int tx, int ty)
{
	struct tile *t = &tiles[tx][ty];

	if(t->is_revealed || t->is_flagged)
		return;

	t->is_revealed = 1;

	if(t->has_bomb) {
		is_game_lost = 1;

		return;
	}

	if(tile_get_surrounding_bombs_count(tx, ty))
		return;

	int x_start = max(0, tx - 1);
	int y_start = max(0, ty - 1);
	int x_end = min(TILES_X, tx + 2);
	int y_end = min(TILES_Y, ty + 2);

	for(int y = y_start; y < y_end; y++) {
		for(int x = x_start; x < x_end; x++) {
			tile_reveal(x, y);
		}
	}
}

static bool is_game_won(void)
{
	int num_bombs = 0;
	int num_unrevealed = 0;

	for(int y = 0; y < TILES_Y; y++) {
		for(int x = 0; x < TILES_X; x++) {
			struct tile t = tiles[x][y];
			num_bombs += t.has_bomb;
			num_unrevealed += !t.is_revealed;
		}
	}

	return (num_bombs == num_unrevealed);
}

void tiles_update(int window_height)
{
	int mouse_pos[2];

	if(is_game_lost)
		return;

	if(is_game_won()) {
		printf("YOU'RE WINNER!\n");
	}

	mouse_pos_get_as_tile(mouse_pos, window_height);

	if(window_mouse_down_left()) {
		tile_reveal(mouse_pos[0], mouse_pos[1]);
		return;
	}

	if(window_mouse_down_right()) {
		struct tile *t = &tiles[mouse_pos[0]][mouse_pos[1]];
		t->is_flagged = !t->is_flagged;
		return;
	}
}

static void tile_draw(int tx, int ty, int win_width, int win_height)
{
	int tile_x = tx * TILE_SIZE;
	int tile_y = ty * TILE_SIZE;

	const float rect[4] = {
		(float)tile_x / (float)win_width,
		(float)tile_y / (float)win_height,
		(float)TILE_SIZE / (float)win_width,
		(float)TILE_SIZE / (float)win_height,
	};

	float verts[4][4] = {
		{rect[0],           rect[1],           0, 1},
		{rect[0] + rect[2], rect[1],           1, 1},
		{rect[0],           rect[1] + rect[3], 0, 0},
		{rect[0] + rect[2], rect[1] + rect[3], 1, 0},
	};

	/*
	 * Normalizing the vertices to OpenGL's coordinate system.
	 */
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 4; j++) {
			float *cur = &verts[j][i];
			*cur *= 2;
			*cur -= 1;
		}
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts,
			GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(tile_shader);

	int num_surrounding = tile_get_surrounding_bombs_count(tx, ty);

	glBindTexture(GL_TEXTURE_2D, tile_tex);

	struct tile t = tiles[tx][ty];

	if(t.is_flagged) {
		glBindTexture(GL_TEXTURE_2D, tile_flagged_tex);
	}

	if(t.is_revealed) {
		glBindTexture(GL_TEXTURE_2D, tile_empty_tex[num_surrounding]);
	}

	if(t.has_bomb) {
		if(t.is_revealed || (is_game_lost && !t.is_revealed))
			glBindTexture(GL_TEXTURE_2D, tile_bomb_tex);
	}

	glDrawElements(GL_TRIANGLES, sizeof(indis) / sizeof(*indis),
			GL_UNSIGNED_INT, indis);
	glBindVertexArray(0);
}

void tiles_draw(int win_width, int win_height)
{
	for(int y = 0; y < TILES_Y; y++) {
		for(int x = 0; x < TILES_X; x++) {
			tile_draw(x, y, win_width, win_height);
		}
	}
}

void tiles_terminate()
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
