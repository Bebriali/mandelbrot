#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "rendering.h"
// #include "benchmarking.h"

vec_t MAX_N_128        = SET1(MAX_N);
vec_t MAX_RADIUS_128   = SET1(MAX_RADIUS);

WSet* WSetCtor(void)
{
    WSet* settings = (WSet*) calloc(1, sizeof(WSet));
    if (settings == NULL)
    {
        printf("error in allocation [for WSet]\n");
        return NULL;
    }
    settings->y_cntr = (int) HEIGHT * 0.5;
    settings->x_cntr = (int) WIDTH  * 0.5; //5 / 8;
    settings->scale  = (float)5e-3;
    settings->step   = 8;

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
                        (*pixels)[(size_t)(y_i * WIDTH + x_i)].color = sf::Color((sf::Uint8)(42), (sf::Uint8)(160), (sf::Uint8)(255));
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
#ifdef FOUR_PACK
    vec_t slip   = SET(3.f, 2.f, 1.f, 0.f);
#endif
#ifdef EIGHT_PACK
    vec_t slip  = SET(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
#endif
    vec_t scale  = SET1((float)settings->scale);
    vec_t x_cntr = SET1((float)settings->x_cntr);
    vec_t y_cntr = SET1((float)settings->y_cntr);

    for (int x_i = 0; x_i < WIDTH; x_i += PACK_SIZE)
    {
        for (int y_i = 0; y_i < HEIGHT; y_i++)
        {
            for (int i = 0; i < evaluate; i++)
            {
                vec_t   n = SET1(0.f);
                vec_t cmp = SET1(1.f);

                vec_t x_vec = SET1((float)x_i); x_vec = ADD(x_vec, slip);
                vec_t y_vec = SET1((float)y_i);

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

                    // cmp = AND(CMP_LE(RAD, MAX_RADIUS_128), SET1(1));
                    cmp = CMP_LE(RAD, MAX_RADIUS_128);

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

// fill 4 pixels in array due to the iteration of their leaving
void SaveIntrinPixels(sf::VertexArray* pixels, int x_i, int y_i, vec_t* n)
{
    alignas(PACK_SIZE * 4) float intpt_itrn[PACK_SIZE] = {};
    STORE(intpt_itrn, *n);

    for (int pack_it = 0; pack_it < PACK_SIZE; pack_it++)
    {
        (*pixels)[(size_t)(y_i * WIDTH + x_i + pack_it)].position = sf::Vector2f(static_cast<float>(x_i + pack_it), static_cast<float>(y_i));
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
