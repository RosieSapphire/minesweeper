#ifndef VEC2_H
#define VEC2_H

struct vec2f_t {
    float x;
    float y;
};

struct vec2i_t {
	int x;
	int y;
};

struct vec2u_t {
	unsigned int x;
	unsigned int y;
};

vec2f_t vec2f_create(const float x, const float y);
vec2f_t vec2f_create_both(const float b);
vec2f_t vec2f_add(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_sub(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_mul(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_scale(const vec2f_t a, const float b);

vec2i_t vec2i_create(const int x, const int y);
vec2i_t vec2i_create_both(const int b);
vec2i_t vec2i_add(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_sub(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_mul(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_scale(const vec2i_t a, const float b);

vec2u_t vec2u_create(const unsigned int x, const unsigned int y);
vec2u_t vec2u_create_both(const unsigned int b);
vec2u_t vec2u_add(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_sub(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_mul(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_scale(const vec2u_t a, const float b);

#endif
