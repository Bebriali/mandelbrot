#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <SFML/Graphics.hpp>

#define MUL(x, y) _mm_mul_ps(x, y)
#define DIV(x, y) _mm_div_ps(x, y)
#define ADD(x, y) _mm_add_ps(x, y)
#define SUB(x, y) _mm_sub_ps(x, y)
#define AND(x, y) _mm_and_ps(x, y)

#define CMP_LE(x, y) _mm_cmple_ps(x, y)
#define CMP_GE(x, y) _mm_cmpge_ps(x, y)
#define  STORE(x, y) _mm_store_ps(x, y)
#define   SET1(x)    _mm_set1_ps(x)
#define    MSK(x)    _mm_movemask_ps(x)

#define SET(x, y, z, t) _mm_set_ps(x, y, z, t)

typedef __m128 vec_t;

// const int MAX_RADIUS    = 20;
const int PACK_SIZE     = 4;
const int WIDTH         = 800;
const int HEIGHT        = 600;

enum color
{
    RED_COL,
    BLUE_COL,
    BLACK_COL,
    PINK_COL,
    WHITE_COL,
    YELLOW_COL
};

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
vec_t MAX_RADIUS_128   = SET1(20.f);
vec_t MAX_N_128        = SET1(1000.f);

struct WSet
{
    int x_cntr;
    int y_cntr;
    float scale;
    int step;
};

void HandleArgs(char** argv, bool* show_flag, int* eval_flag, bool* intrin_flag);
WSet* WSetCtor(void);
void CountPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels);
void CountIntrinPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels);
void SaveIntrinPixels(sf::VertexArray* pixels, int x_i, int y_i, vec_t* n);
void HandleKeyInt(WSet* settings);
float Radius(float x, float y);
vec_t RadiusIntrin(vec_t x, vec_t y);


int main(int argc, char* argv[])
{
    bool SHOW     = true;
    int  EVALUATE = 1;
    bool INTRIN   = false;
    if (argc > 1)
    {
        if (argc > 3)
        {
            HandleArgs(argv, &SHOW, &EVALUATE, &INTRIN);
        }
        else if (argc > 2)
        {
            HandleArgs(argv, &SHOW, &EVALUATE, NULL);
        }
        else
        {
            HandleArgs(argv, &SHOW, NULL, NULL);
        }
    }

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "mandelbrot set");
    sf::VertexArray  pixels(sf::Points, WIDTH * HEIGHT);

    WSet* settings = WSetCtor();
    if (settings == NULL)
    {
        return PTR_ERROR;
    }

    bool RENDERING = true;

    while (RENDERING)
    {
        while (window.isOpen())
        {
            sf::Event event;
            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q))
            {
                RENDERING = false;
                break;
            }

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            if (INTRIN)
            {
                CountIntrinPixels(SHOW, EVALUATE, settings, &pixels);
            }
            else
            {
                CountPixels(SHOW, EVALUATE, settings, &pixels);
            }

            window.clear();
            window.draw (pixels);
            window.display();

            HandleKeyInt(settings);
        }
    }


    return 0;
}

WSet* WSetCtor(void)
{
    WSet* settings = (WSet*) calloc(1, sizeof(WSet));
    if (settings == NULL)
    {
        printf("error in allocation [for Wset]\n");
        return NULL;
    }
    settings->y_cntr = (int) HEIGHT * 0.5;
    settings->x_cntr = (int) WIDTH  * 0.5; //5 / 8;
    settings->scale  = 0.005;
    settings->step   = 2;

    return settings;
}


void HandleKeyInt(WSet* settings)
{
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::P))     settings->scale  /= (float)1.05;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::M))     settings->scale  *= (float)1.05;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))    settings->y_cntr -= settings->step;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))  settings->y_cntr += settings->step;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))  settings->x_cntr -= settings->step;
    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right)) settings->x_cntr += settings->step;

    return ;
}

float Radius(float x, float y)
{
    return x * x + y * y;
}

void CountPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels)
{
    for (int x_i = 0; x_i < WIDTH; x_i++)
    {
        for (int y_i = 0; y_i < HEIGHT; y_i++)
        {
            for (int i = 0; i < evaluate; i++)
            {
                int N = 0;
                float y1 = settings->scale * (float)(y_i - settings->y_cntr);
                float x1 = settings->scale * (float)(x_i - settings->x_cntr);


                float x = 0;
                float y = 0;

                float RAD = Radius(x, y);

                while(RAD <= MAX_RADIUS && N <= MAX_N)
                {
                    float x_2 = x * x;
                    float y_2 = y * y;
                    float xy  = x * y;

                    x = x_2 - y_2 + x1;
                    y = xy  +  xy + y1;
                    RAD = Radius(x, y);

                    N++;
                }

                if (show)
                {
                    (*pixels)[(size_t)(y_i * WIDTH + x_i)].position = sf::Vector2f(static_cast<float>(x_i), static_cast<float>(y_i));

                    if (N <= MAX_N)
                    {
                        if (N <= 30)
                            (*pixels)[(size_t)(y_i * WIDTH + x_i)].color = sf::Color(0, 0, 0);

                        else if (N > 30 && N <= 40)
                            (*pixels)[(size_t)(y_i * WIDTH + x_i)].color = sf::Color((sf::Uint8)(255), (sf::Uint8)(128), (sf::Uint8)(128));
                        else
                            (*pixels)[(size_t)(y_i * WIDTH + x_i)].color = sf::Color((sf::Uint8)(255), 0, 0);

                    }
                    else
                    {
                        // if (RAD < 0.0001 * MAX_RADIUS)
                            (*pixels)[(size_t)(y_i * WIDTH + x_i)].color = sf::Color((sf::Uint8)(42), (sf::Uint8)(160), (sf::Uint8)(255));
                        // else if (RAD >= 0.0001 * MAX_RADIUS && RAD < 0.0005 * MAX_RADIUS)
                        //     pix_col = WHITE_COL;
                        // else
                        //     pix_col = YELLOW_COL;
                    }

                }
            }
        }
    }


    return ;
}

vec_t RadiusIntrin(vec_t x, vec_t y)
{
    return ADD(MUL(x, x), MUL(y, y));
}

void CountIntrinPixels(bool show, int evaluate, WSet* settings, sf::VertexArray* pixels)
{
    vec_t slip   = SET(0, 1, 2, 3);
    vec_t scale  = SET1(settings->scale);
    vec_t x_cntr = SET1(settings->x_cntr);
    vec_t y_cntr = SET1(settings->y_cntr);

    for (int x_i = 0; x_i < WIDTH; x_i += PACK_SIZE)
    {
        for (int y_i = 0; y_i < HEIGHT; y_i++)
        {
            for (int i = 0; i < evaluate; i++)
            {
                vec_t   n = SET1(0.f);
                vec_t cmp = SET1(1.f);

                vec_t x_vec = SET1(x_i); x_vec = ADD(x_vec, slip);
                vec_t y_vec = SET1(y_i);

                vec_t x_1 = MUL(scale, SUB(x_vec, x_cntr));
                vec_t y_1 = MUL(scale, SUB(y_vec, y_cntr));

                vec_t x   = SET1(0.f);
                vec_t y   = SET1(0.f);

                vec_t RAD = RadiusIntrin(x, y); //ADD(MUL(x, x), MUL(y, y));

                int N = 0;

                while(N <= MAX_N)
                {
                    vec_t x_2 = MUL(x, x);
                    vec_t y_2 = MUL(y, y);
                    vec_t xy  = MUL(x, y);

                    x = ADD(SUB(x_2, y_2), x_1);
                    // x = x_2 - y_2 + x1;

                    y = ADD(ADD(xy,  xy),  y_1);
                    // y = xy  +  xy + y1;

                    RAD = ADD(x_2, y_2);
                    // x * x + y * y

                    cmp = ADD(CMP_LE(RAD, MAX_RADIUS_128), SET1(1.f));

                    int rad_stat = MSK(cmp);
                    if (!rad_stat)
                    {
                        break;
                    }

                    cmp = AND(cmp, SET1(1));
                    n = ADD(n, cmp);
                    N++;
                }
                SaveIntrinPixels(pixels, x_i, y_i, &n);
            }
        }
    }
}

void SaveIntrinPixels(sf::VertexArray* pixels, int x_i, int y_i, vec_t* n)
{
    // fill 4 pixels in array due to the iteration of their leaving
    alignas(PACK_SIZE * 4) float intpt_itrn[PACK_SIZE] = {};
    STORE(intpt_itrn, *n);
    // if (intpt_itrn[0]

    for (int pack_it = 0; pack_it < PACK_SIZE; pack_it++)
    {
        (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].position = sf::Vector2f(static_cast<float>(x_i + pack_it), static_cast<float>(y_i));
        // (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].color    = sf::Color((sf::Uint8)(256 - ((int)intpt_itrn[pack_it]) * 16), 0, (sf::Uint8)(256 - ((int)intpt_itrn[pack_it]) * 16));
        if (intpt_itrn[pack_it] <= MAX_N)
        {
            if (intpt_itrn[pack_it] <= 30)
                (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].color = sf::Color(0, 0, 0);

            else if (intpt_itrn[pack_it] > 30 && intpt_itrn[pack_it] <= 40)
                (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].color = sf::Color((sf::Uint8)(255), (sf::Uint8)(128), (sf::Uint8)(128));
            else
                (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].color = sf::Color((sf::Uint8)(255), 0, 0);

        }
        else
        {
            // printf("intpt_itrn[%d] = %f\n", x_i + pack_it, intpt_itrn[pack_it]);
            (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].color = sf::Color((sf::Uint8)(42), (sf::Uint8)(160), (sf::Uint8)(255));
        }
    }
}



void HandleArgs(char** argv, bool* show_flag, int* eval_flag, bool* intrin_flag)
{
    if (strcmp(argv[1], "--show") != 0)
    {
        *show_flag = false;
    }
    if (eval_flag != NULL)
    {
        for (int i = 0; argv[2][i] != '\0'; i++)
        {
            if (argv[2][i] == '=')
            {
                *eval_flag = 0;
                for (int j = i + 1; argv[2][j] <='9' && argv[2][j] >= '0'; j++)
                {
                    *eval_flag = *eval_flag * 10 + (int)argv[2][j] - (int)'0';
                }
            }
        }
    }
    if (intrin_flag != NULL)
    {
        if (strcmp(argv[3], "--intrin-on") == 0)
        {
            *intrin_flag = true;
        }
    }

    return ;
}
