#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <assert.h>

#include "rendering.h"
#include "benchmarking.h"

tick_point** OpenWindow(bool SHOW, int Evaluate, render_func_t render);
const char* count_filename  = "data/count.txt";
const char* render_filename = "data/render.txt";

ticks_t Benchmarking(bool SHOW, int EVALUATE, bool INTRIN)
{
    tick_point** ticks_arr = NULL;
    if (INTRIN)
        ticks_arr = OpenWindow(SHOW, EVALUATE, CountIntrinPixels);
    else
        ticks_arr = OpenWindow(SHOW, EVALUATE, CountPixels);

    if (ticks_arr == NULL)
    {
        printf("error_ptr [ticks_arr]\n");
        return 0;
    }

    // ConsoleOutput(ticks_arr, INTRIN, EVALUATE);

    printf("going to dump\n");
    FileOutput(ticks_arr, INTRIN, EVALUATE);

    return 0;
}

tick_point** OpenWindow(bool SHOW, int EVALUATE, render_func_t render)
{
    tick_point** ticks_arr = (tick_point**) calloc(TICKS_ARR_SIZE, sizeof(tick_point*));
    if (ticks_arr == NULL)
    {
        printf("error in callocation [for ticks_arr]\n");
        return NULL;
    }
    size_t rndr_it = 0;
    for (rndr_it; rndr_it < TICKS_ARR_SIZE; rndr_it++)
    {
        ticks_arr[rndr_it] = (tick_point*) calloc(1, sizeof(tick_point));
        if (ticks_arr[rndr_it] == NULL)
        {
            printf("error callocation [for tick_point*]\n");
            return NULL;
        }
    }
    rndr_it = 0;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "mandelbrot set");
    sf::VertexArray  pixels(sf::Points, WIDTH * HEIGHT);

    WSet* settings = WSetCtor();
    if (settings == NULL)
    {
        return NULL;
    }

    bool RENDERING = true;

    while (RENDERING)
    {
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            ticks_t ticks_begin = __rdtsc() * (-1);

            printf("counting points: ...");
            render(SHOW, EVALUATE, settings, &pixels);
            printf("\b\b\bcounted\n");

            ticks_arr[rndr_it]->count = __rdtsc() + ticks_begin;

            if (ticks_arr[rndr_it] == NULL)
            {
                printf("NULLPTR ticks\n");
            }
            else
            {
                printf("TICKS_PTR[%d] = %ld\n", rndr_it, ticks_arr[rndr_it]);
            }
            printf("count = %I64ld, rendering = %I64ld\n", ticks_arr[rndr_it]->count, ticks_arr[rndr_it]->render);

            window.clear();
            window.draw (pixels);
            window.display();

            HandleKeyInt(settings);

            ticks_arr[rndr_it]->render = __rdtsc() - ticks_arr[rndr_it]->count + ticks_begin;
            rndr_it += (rndr_it < TICKS_ARR_SIZE - 1) ? 1 : 0;

            if (sf::Keyboard::isKeyPressed (sf::Keyboard::Q))
            {
                RENDERING = false;
                break;
            }
        }
    }

    printf("ticks_arr ptr = %ld\n", ticks_arr);
    return ticks_arr;
}

void FileOutput(tick_point** ticks_arr, bool INTRIN, int EVALUATE)
{
    assert(ticks_arr);

    printf("opening files\n");
    FILE* count  = fopen(count_filename,  "wb");
    FILE* render = fopen(render_filename, "wb");

    if (count == NULL || render == NULL)
    {
        printf("error_ptr [count || render]\n");
        return ;
    }

    printf("dumping results: ...");

    fprintf(count,  "INTRIN = %s", INTRIN ? "ON\n" : "OFF\n");
    fprintf(render, "INTRIN = %s", INTRIN ? "ON\n" : "OFF\n");

    for (size_t i = 0; ticks_arr[i]->count && i < TICKS_ARR_SIZE; i++)
    {
        fprintf(count,  "%ld\n", ticks_arr[i]->count);
        fprintf(render, "%ld\n", ticks_arr[i]->render);
    }

    fclose(count);
    fclose(render);

    printf("\b\b\bdumped\n");

    return ;
}
