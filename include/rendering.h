#ifndef RENDERING_H
#define RENDERING_H

#include <xmmintrin.h>
#include <immintrin.h>
#include <SFML/Graphics.hpp>

// #define FOUR_PACK
#define EIGHT_PACK

#ifdef FOUR_PACK

#define    MUL(x, y)    _mm_mul_ps(x, y)
#define    DIV(x, y)    _mm_div_ps(x, y)
#define    ADD(x, y)    _mm_add_ps(x, y)
#define    SUB(x, y)    _mm_sub_ps(x, y)
#define    AND(x, y)    _mm_and_ps(x, y)

#define CMP_LE(x, y)    _mm_cmple_ps(x, y)
#define CMP_GE(x, y)    _mm_cmpge_ps(x, y)
#define  STORE(x, y)    _mm_store_ps(x, y)
#define   SET1(x)       _mm_set1_ps(x)
#define    MSK(x)       _mm_movemask_ps(x)

#define SET(x, y, z, t) _mm_set_ps(x, y, z, t)

typedef __m128 vec_t;

const int PACK_SIZE     = 4;
const int HEIGHT        = 600;

#endif

#ifdef EIGHT_PACK

#define    MUL(x, y)    _mm256_mul_ps(x, y)
#define    DIV(x, y)    _mm256_div_ps(x, y)
#define    ADD(x, y)    _mm256_add_ps(x, y)
#define    SUB(x, y)    _mm256_sub_ps(x, y)
#define    AND(x, y)    _mm256_and_ps(x, y)

#define CMP_LE(x, y)    _mm256_cmp_ps(x, y, _CMP_LT_OS)
#define CMP_GE(x, y)    _mm256_cmpge_ps(x, y)
#define  STORE(x, y)    _mm256_store_ps(x, y)
#define   SET1(x)       _mm256_set1_ps(x)
#define    MSK(x)       _mm256_movemask_ps(x)

#define SET(x, y, z, t, a, b, c, d) _mm256_set_ps(x, y, z, t, a, b, c, d)

typedef __m256 vec_t;

const int PACK_SIZE     = 8;
const int HEIGHT        = 604;

#endif


const int WIDTH         = 800;

enum errors
{
    NO_ERRORS,
    PTR_ERROR
};

enum limit_conditions
{
    MAX_N      = 100,
    MAX_RADIUS = 20
};

extern vec_t MAX_N_128;
extern vec_t MAX_RADIUS_128;

struct WSet
{
    int x_cntr;
    int y_cntr;
    float scale;
    int step;
};

typedef void (*render_func_t) (bool show, int evaluate, WSet* settings, sf::VertexArray* pixels);

// void HandleArgs(char** argv, bool* show_flag, int* eval_flag, bool* intrin_flag);

WSet* WSetCtor(void);
void HandleKeyInt(WSet* settings);

void CountPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels);
float Radius(float x, float y);

void CountIntrinPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels);
void SaveIntrinPixels(sf::VertexArray* pixels, int x_i, int y_i, vec_t* n);
vec_t RadiusIntrin(vec_t x, vec_t y);

#endif
