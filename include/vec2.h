#ifndef VEC2_H
#define VEC2_H

#include <stdint.h>

typedef struct {
    float x;
    float y;
} vec2f_t;

typedef struct {
	int32_t x;
	int32_t y;
} vec2i_t;

typedef struct {
	uint32_t x;
	uint32_t y;
} vec2u_t;

vec2f_t vec2f_create(const float x, const float y);
vec2f_t vec2f_create_both(const float b);
vec2f_t vec2f_add(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_sub(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_mul(const vec2f_t a, const vec2f_t b);
vec2f_t vec2f_scale(const vec2f_t a, const float b);

vec2i_t vec2i_create(const int32_t x, const int32_t y);
vec2i_t vec2i_create_both(const int32_t b);
vec2i_t vec2i_add(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_sub(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_mul(const vec2i_t a, const vec2i_t b);
vec2i_t vec2i_scale(const vec2i_t a, const float b);

vec2u_t vec2u_create(const uint32_t x, const uint32_t y);
vec2u_t vec2u_create_both(const uint32_t b);
vec2u_t vec2u_add(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_sub(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_mul(const vec2u_t a, const vec2u_t b);
vec2u_t vec2u_scale(const vec2u_t a, const float b);

#endif
