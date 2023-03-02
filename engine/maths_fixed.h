
// Fixed point math library, modified from
// https://github.com/Simponic/Simple3DEngine

#pragma once

#include "platform.h"

#define FIX_SHIFT       8
#define HALF_FIX_SHIFT  4
#define FIX_SCALE       ( 1 << FIX_SHIFT )
#define FIX_SCALE_FLOAT ((float)(FIX_SCALE))
#define TO_FIXED(v)     ((FIXED)(v * FIX_SCALE_FLOAT))

typedef s32 FIXED;
typedef struct vec2fx
{
	FIXED x;
	FIXED y;
} vec2fx;

typedef struct vec3fx
{
	FIXED x;
	FIXED y;
	FIXED z;
} vec3fx;

typedef struct lineequ
{
	FIXED slope;
	FIXED intercept;
} lineequ;

static inline FIXED fixed_OverMultiply(FIXED a, FIXED b) {
	// This should multiply two fixed-point numbers sacrificing a little
	// accuracy in exchange for less chance of an overflow

	return ((a >> HALF_FIX_SHIFT)* (b >> HALF_FIX_SHIFT));
}

static inline FIXED fixed_multiply(FIXED a, FIXED b) {
	// Multiply two fixed numbers. Possibility of overflow.
	return (a * b) >> FIX_SHIFT;
}

static inline FIXED fixed_divide(FIXED a, FIXED b) {
	if (b != 0) {
		return (a * FIX_SCALE) / b;
	}
	else {
		return 100000 << FIX_SHIFT;
	}
}

static inline FIXED float_to_fixed(float a) {
	// Convert a float to fixed point
	return ((FIXED)(a * FIX_SCALE_FLOAT));
}

static inline float fixed_to_float(FIXED a) {
	// Convert fixed point to float
	return (a / FIX_SCALE_FLOAT);
}

static inline FIXED int_to_fixed(s32 a) {
	// Convert a float to fixed point
	return ((FIXED)a) << FIX_SHIFT;
}

static inline s32 fixed_to_int(FIXED a) {
	// Convert fixed point to float
	return (s32)(a / FIX_SCALE_FLOAT);
}

static inline void swapFixed(FIXED* a, FIXED* b) {
	// Swap two fixed point integer pointers
	FIXED temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

static inline FIXED fixed_sqrt(FIXED a, int iterations) {
	// Calculate square root of a fixed-point number using Binary-Search
	FIXED low = 0;
	FIXED high = a;
	FIXED mid;
	FIXED midSquared;
	for (int i = 0; i < iterations; i++) {
		mid = fixed_divide((low + high), 2 << FIX_SHIFT);
		midSquared = fixed_OverMultiply(mid, mid);
		if (midSquared == a) {
			return mid;
		}
		else if (midSquared > a) {
			high = mid;
		}
		else {
			low = mid;
		}
	}
	return mid;
}

static inline FIXED fixed_crossproduct(FIXED x1, FIXED y1, FIXED x2, FIXED y2)
{
	return fixed_multiply(x1, y2) - fixed_multiply(x2, y1);
}

static inline FIXED fixed_pointonside(FIXED px, FIXED py, FIXED x0, FIXED y0, FIXED x1, FIXED y1)
{
	return fixed_crossproduct(x1 - x0, y1 - y0, px - x0, py - y0);
}

static inline vec2fx fixed_intersect(FIXED x1, FIXED y1, FIXED x2, FIXED y2, FIXED x3, FIXED y3, FIXED x4, FIXED y4)
{
	vec2fx result;

	result.x = fixed_crossproduct(fixed_crossproduct(x1, y1, x2, y2), (x1)-(x2), fixed_crossproduct(x3, y3, x4, y4), (x3)-(x4)) / fixed_crossproduct((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4));
	result.y = fixed_crossproduct(fixed_crossproduct(x1, y1, x2, y2), (y1)-(y2), fixed_crossproduct(x3, y3, x4, y4), (y3)-(y4)) / fixed_crossproduct((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4));

	return result;
}
