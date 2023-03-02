
#pragma once

#include "platform.h"
#include "maths_fixed.h"

#define maths_min(a,b)                                      (((a) < (b)) ? (a) : (b))
#define maths_max(a,b)                                      (((a) > (b)) ? (a) : (b))
#define maths_clamp(a, mi, ma)                              maths_min(maths_max(a,mi),ma)
#define maths_overlap(a0,a1,b0,b1)                          (maths_min(a0,a1) <= maths_max(b0,b1) && maths_min(b0,b1) <= maths_max(a0,a1))
#define maths_rectoverlap(x0, y0, x1, y1, x2, y2, x3, y3)   (maths_overlap(x0,x1,x2,x3) && maths_overlap(y0,y1,y2,y3))

typedef struct vec2
{
    s32 x;
    s32 y;
} vec2;

typedef struct vec3
{
    s32 x;
    s32 y;
    s32 z;
} vec3;

typedef struct vec2f
{
	float x;
	float y;
} vec2f;

typedef struct vec3f
{
	float x;
	float y;
	float z;
} vec3f;

typedef struct lineequf
{
	float slope;
	float intercept;
} lineequf;

typedef struct randomconfig
{
    vu8 a;
    vu8 b;
    vu8 c;
    vu8 d;
} randomconfig;
extern randomconfig randomsource;

// Fixed Point (24.8) maths in s32
FIXED sini(s32 degrees);
FIXED cosi(s32 degrees);

// Random Number generation
void random_init( u8 seed );
u8 random_next();

lineequ fixed_lineequation(FIXED x1, FIXED y1, FIXED x2, FIXED y2);
u8 fixed_linesintersect(FIXED x1, FIXED y1, FIXED x2, FIXED y2, FIXED x3, FIXED y3, FIXED x4, FIXED y4, vec2f* location);

lineequf float_lineequation(float x1, float y1, float x2, float y2);
u8 float_linesintersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, vec2f* location);


float easeoutback(float Time, float Begin, float Change, float Duration, float Swing);
float easeoutcubic(float Time, float Begin, float Change, float Duration);
