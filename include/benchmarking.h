#ifndef BENCHMARKING_H
#define BENCHMARKING_H

#include <cstdint>
// #include "rendering.h"

typedef uint64_t ticks_t;

typedef struct
{
    ticks_t count;
    ticks_t render;
} tick_point;

const size_t TICKS_ARR_SIZE = 1000;

ticks_t Benchmarking(bool SHOW, int EVALUATE, bool INTRIN);

// void ConsoleOutput(tick_point** ticks_arr, bool INTRIN, int EVALUATE);
void FileOutput(tick_point** ticks_arr, bool INTRIN, int EVALUATE);

#endif
