
#include "maths.h"


randomconfig randomsource = { 0x5E, 0x00, 0x00, 0x00 };

const FIXED sine[] = {
      0 ,    4 ,    9 ,   13 ,   18 ,   22 ,   27 ,   31 ,   36 ,   40 ,   44 ,   49 ,   53 ,   58 ,   62 ,   66 ,   71 ,   75 ,   79 ,   83
 ,   88 ,   92 ,   96 ,  100 ,  104 ,  108 ,  112 ,  116 ,  120 ,  124 ,  128 ,  132 ,  136 ,  139 ,  143 ,  147 ,  150 ,  154 ,  158 ,  161
 ,  165 ,  168 ,  171 ,  175 ,  178 ,  181 ,  184 ,  187 ,  190 ,  193 ,  196 ,  199 ,  202 ,  204 ,  207 ,  210 ,  212 ,  215 ,  217 ,  219
 ,  222 ,  224 ,  226 ,  228 ,  230 ,  232 ,  234 ,  236 ,  237 ,  239 ,  241 ,  242 ,  243 ,  245 ,  246 ,  247 ,  248 ,  249 ,  250 ,  251
 ,  252 ,  253 ,  254 ,  254 ,  255 ,  255 ,  255 ,  256 ,  256 ,  256 ,  256 ,  256 ,  256 ,  256 ,  255 ,  255 ,  255 ,  254 ,  254 ,  253
 ,  252 ,  251 ,  250 ,  249 ,  248 ,  247 ,  246 ,  245 ,  243 ,  242 ,  241 ,  239 ,  237 ,  236 ,  234 ,  232 ,  230 ,  228 ,  226 ,  224
 ,  222 ,  219 ,  217 ,  215 ,  212 ,  210 ,  207 ,  204 ,  202 ,  199 ,  196 ,  193 ,  190 ,  187 ,  184 ,  181 ,  178 ,  175 ,  171 ,  168
 ,  165 ,  161 ,  158 ,  154 ,  150 ,  147 ,  143 ,  139 ,  136 ,  132 ,  128 ,  124 ,  120 ,  116 ,  112 ,  108 ,  104 ,  100 ,   96 ,   92
 ,   88 ,   83 ,   79 ,   75 ,   71 ,   66 ,   62 ,   58 ,   53 ,   49 ,   44 ,   40 ,   36 ,   31 ,   27 ,   22 ,   18 ,   13 ,    9 ,    4
 ,    0 ,   -4 ,   -9 ,  -13 ,  -18 ,  -22 ,  -27 ,  -31 ,  -36 ,  -40 ,  -44 ,  -49 ,  -53 ,  -58 ,  -62 ,  -66 ,  -71 ,  -75 ,  -79 ,  -83
 ,  -88 ,  -92 ,  -96 , -100 , -104 , -108 , -112 , -116 , -120 , -124 , -128 , -132 , -136 , -139 , -143 , -147 , -150 , -154 , -158 , -161
 , -165 , -168 , -171 , -175 , -178 , -181 , -184 , -187 , -190 , -193 , -196 , -199 , -202 , -204 , -207 , -210 , -212 , -215 , -217 , -219
 , -222 , -224 , -226 , -228 , -230 , -232 , -234 , -236 , -237 , -239 , -241 , -242 , -243 , -245 , -246 , -247 , -248 , -249 , -250 , -251
 , -252 , -253 , -254 , -254 , -255 , -255 , -255 , -256 , -256 , -256 , -256 , -256 , -256 , -256 , -255 , -255 , -255 , -254 , -254 , -253
 , -252 , -251 , -250 , -249 , -248 , -247 , -246 , -245 , -243 , -242 , -241 , -239 , -237 , -236 , -234 , -232 , -230 , -228 , -226 , -224
 , -222 , -219 , -217 , -215 , -212 , -210 , -207 , -204 , -202 , -199 , -196 , -193 , -190 , -187 , -184 , -181 , -178 , -175 , -171 , -168
 , -165 , -161 , -158 , -154 , -150 , -147 , -143 , -139 , -136 , -132 , -128 , -124 , -120 , -116 , -112 , -108 , -104 , -100 ,  -96 ,  -92
 ,  -88 ,  -83 ,  -79 ,  -75 ,  -71 ,  -66 ,  -62 ,  -58 ,  -53 ,  -49 ,  -44 ,  -40 ,  -36 ,  -31 ,  -27 ,  -22 ,  -18 ,  -13 ,   -9 ,   -4
 };

const FIXED cosine[] = {
    256 ,  256 ,  256 ,  256 ,  255 ,  255 ,  255 ,  254 ,  254 ,  253 ,  252 ,  251 ,  250 ,  249 ,  248 ,  247 ,  246 ,  245 ,  243 ,  242
 ,  241 ,  239 ,  237 ,  236 ,  234 ,  232 ,  230 ,  228 ,  226 ,  224 ,  222 ,  219 ,  217 ,  215 ,  212 ,  210 ,  207 ,  204 ,  202 ,  199
 ,  196 ,  193 ,  190 ,  187 ,  184 ,  181 ,  178 ,  175 ,  171 ,  168 ,  165 ,  161 ,  158 ,  154 ,  150 ,  147 ,  143 ,  139 ,  136 ,  132
 ,  128 ,  124 ,  120 ,  116 ,  112 ,  108 ,  104 ,  100 ,   96 ,   92 ,   88 ,   83 ,   79 ,   75 ,   71 ,   66 ,   62 ,   58 ,   53 ,   49
 ,   44 ,   40 ,   36 ,   31 ,   27 ,   22 ,   18 ,   13 ,    9 ,    4 ,    0 ,   -4 ,   -9 ,  -13 ,  -18 ,  -22 ,  -27 ,  -31 ,  -36 ,  -40
 ,  -44 ,  -49 ,  -53 ,  -58 ,  -62 ,  -66 ,  -71 ,  -75 ,  -79 ,  -83 ,  -88 ,  -92 ,  -96 , -100 , -104 , -108 , -112 , -116 , -120 , -124
 , -128 , -132 , -136 , -139 , -143 , -147 , -150 , -154 , -158 , -161 , -165 , -168 , -171 , -175 , -178 , -181 , -184 , -187 , -190 , -193
 , -196 , -199 , -202 , -204 , -207 , -210 , -212 , -215 , -217 , -219 , -222 , -224 , -226 , -228 , -230 , -232 , -234 , -236 , -237 , -239
 , -241 , -242 , -243 , -245 , -246 , -247 , -248 , -249 , -250 , -251 , -252 , -253 , -254 , -254 , -255 , -255 , -255 , -256 , -256 , -256
 , -256 , -256 , -256 , -256 , -255 , -255 , -255 , -254 , -254 , -253 , -252 , -251 , -250 , -249 , -248 , -247 , -246 , -245 , -243 , -242
 , -241 , -239 , -237 , -236 , -234 , -232 , -230 , -228 , -226 , -224 , -222 , -219 , -217 , -215 , -212 , -210 , -207 , -204 , -202 , -199
 , -196 , -193 , -190 , -187 , -184 , -181 , -178 , -175 , -171 , -168 , -165 , -161 , -158 , -154 , -150 , -147 , -143 , -139 , -136 , -132
 , -128 , -124 , -120 , -116 , -112 , -108 , -104 , -100 ,  -96 ,  -92 ,  -88 ,  -83 ,  -79 ,  -75 ,  -71 ,  -66 ,  -62 ,  -58 ,  -53 ,  -49
 ,  -44 ,  -40 ,  -36 ,  -31 ,  -27 ,  -22 ,  -18 ,  -13 ,   -9 ,   -4 ,    0 ,    4 ,    9 ,   13 ,   18 ,   22 ,   27 ,   31 ,   36 ,   40
 ,   44 ,   49 ,   53 ,   58 ,   62 ,   66 ,   71 ,   75 ,   79 ,   83 ,   88 ,   92 ,   96 ,  100 ,  104 ,  108 ,  112 ,  116 ,  120 ,  124
 ,  128 ,  132 ,  136 ,  139 ,  143 ,  147 ,  150 ,  154 ,  158 ,  161 ,  165 ,  168 ,  171 ,  175 ,  178 ,  181 ,  184 ,  187 ,  190 ,  193
 ,  196 ,  199 ,  202 ,  204 ,  207 ,  210 ,  212 ,  215 ,  217 ,  219 ,  222 ,  224 ,  226 ,  228 ,  230 ,  232 ,  234 ,  236 ,  237 ,  239
 ,  241 ,  242 ,  243 ,  245 ,  246 ,  247 ,  248 ,  249 ,  250 ,  251 ,  252 ,  253 ,  254 ,  254 ,  255 ,  255 ,  255 ,  256 ,  256 ,  256
 };

FIXED sini(s32 degrees)
{
    s32 deg = degrees;
    if(deg < 0)
    {
        while(deg < 0)
        {
            deg += 360;
        }
    }
    if(deg >= 360)
    {
        deg = deg % 360;
    }
    return sine[deg];
}

FIXED cosi(s32 degrees)
{
    s32 deg = degrees;
    if(deg < 0)
    {
        while(deg < 0)
        {
            deg += 360;
        }
    }
    if(deg >= 360)
    {
        deg = deg % 360;
    }
    return cosine[deg];
}

// Random Number Generator, adapted from
// http://burtleburtle.net/bob/rand/smallprng.html

#define rot(x,k) ( ( x << k ) | ( x >> (8 - k) ) )

void random_init( u8 seed )
{
    u8 i;
    randomsource.a = 0x5E;
    randomsource.b = randomsource.c = randomsource.d = seed;
    for (i=0; i<20; ++i)
    {
        random_next();
    }
}

u8 random_next()
{
    u8 e = randomsource.a - rot(randomsource.b, 5);
    randomsource.a = randomsource.b ^ rot(randomsource.c, 3);
    randomsource.b = randomsource.c + randomsource.d;
    randomsource.c = randomsource.d + e;
    randomsource.d = e + randomsource.a;
    return randomsource.d;
}


lineequ fixed_lineequation(FIXED x1, FIXED y1, FIXED x2, FIXED y2)
{
    lineequ result;
    if (x1 == x2)
    {
        result.slope = TO_FIXED(100000);
    }
    else
    {
        result.slope = fixed_divide(y2 - y1, x2 - x1);
    }
    result.intercept = y1 - fixed_multiply(result.slope, x1);
    return result;
}

u8 fixed_linesintersect(FIXED x1, FIXED y1, FIXED x2, FIXED y2, FIXED x3, FIXED y3, FIXED x4, FIXED y4, vec2f* location)
{
    lineequ equ1 = fixed_lineequation(x1, y1, x2, y2);
    lineequ equ2 = fixed_lineequation(x3, y3, x4, y4);

    if (x1 == x2)
    {
        if ((x3 <= x1 && x4 >= x1) || (x3 >= x1 && x4 <= x1))
        {
            FIXED equ2y = fixed_multiply(equ2.slope, x1) + equ2.intercept;
            if ((equ2y >= y1 && equ2y <= y2) || (equ2y <= y1 && equ2y >= y2))
            {
                if (location != NULL)
                {
                    location->x = x1;
                    location->y = equ2y;
                }
                return 1;
            }
        }
    }
    else if (x3 == x4)
    {
        if ((x1 <= x3 && x2 >= x3) || (x1 >= x3 && x2 <= x3))
        {
            FIXED equ1y = fixed_multiply(equ1.slope, x3) + equ1.intercept;
            if ((equ1y >= y3 && equ1y <= y4) || (equ1y <= y3 && equ1y >= y4))
            {
                if (location != NULL)
                {
                    location->x = x3;
                    location->y = equ1y;
                }
                return 1;
            }
        }
    }
    else
    {
        FIXED x = fixed_divide(equ2.intercept - equ1.intercept, equ1.slope - equ2.slope);
        if (x >= maths_min(x1, x2) && x <= maths_max(x1, x2))
        {
            if (x >= maths_min(x3, x4) && x <= maths_max(x3, x4))
            {
                if (location != NULL)
                {
                    location->x = x;
                    location->y = fixed_multiply(equ1.slope, x) + equ1.intercept; // (m1 * x) + b1;
                }
                return 1;
            }
        }
    }

    return 0;
}

lineequf float_lineequation(float x1, float y1, float x2, float y2)
{
    lineequf result;
    if (x1 == x2)
    {
        result.slope = 100000.0f;
    }
    else
    {
        result.slope = fixed_divide(y2 - y1, x2 - x1);
    }
    result.intercept = y1 - (result.slope * x1);
    return result;
}

u8 float_linesintersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, vec2f* location)
{
    lineequf equ1 = float_lineequation(x1, y1, x2, y2);
    lineequf equ2 = float_lineequation(x3, y3, x4, y4);

    if (x1 == x2)
    {
        if ((x3 <= x1 && x4 >= x1) || (x3 >= x1 && x4 <= x1))
        {
            float equ2y = (equ2.slope * x1) + equ2.intercept;
            if ((equ2y >= y1 && equ2y <= y2) || (equ2y <= y1 && equ2y >= y2))
            {
                if (location != NULL)
                {
                    location->x = x1;
                    location->y = equ2y;
                }
                return 1;
            }
        }
    }
    else if (x3 == x4)
    {
        if ((x1 <= x3 && x2 >= x3) || (x1 >= x3 && x2 <= x3))
        {
            float equ1y = (equ1.slope * x3) + equ1.intercept;
            if ((equ1y >= y3 && equ1y <= y4) || (equ1y <= y3 && equ1y >= y4))
            {
                if (location != NULL)
                {
                    location->x = x3;
                    location->y = equ1y;
                }
                return 1;
            }
        }
    }
    else
    {
        float x = (equ2.intercept - equ1.intercept) / (equ1.slope - equ2.slope);
        if (x >= maths_min(x1, x2) && x <= maths_max(x1, x2))
        {
            if (x >= maths_min(x3, x4) && x <= maths_max(x3, x4))
            {
                if (location != NULL)
                {
                    location->x = x;
                    location->y = (equ1.slope * x) + equ1.intercept; // (m1 * x) + b1;
                }
                return 1;
            }
        }
    }

    return 0;
}

float easeoutback(float Time, float Begin, float Change, float Duration, float Swing)
{
    float time = Time;
    if (Swing == 0)
    {
        Swing = 1.70158f;
    }
    return Change * ((time = time / Duration - 1) * time * ((Swing + 1) * time + Swing) + 1) + Begin;
}

float easeoutcubic(float Time, float Begin, float Change, float Duration)
{
	return Change * ((Time = Time / Duration - 1) * Time * Time + 1) + Begin;
}
