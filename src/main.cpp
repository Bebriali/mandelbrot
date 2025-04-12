#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>
#include <xmmintrin.h>

// #include "rendering.h"
#include "benchmarking.h"

#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"


void HandleArgs(char** argv, bool* show_flag, int* eval_flag, bool* intrin_flag);

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

    Benchmarking(SHOW, EVALUATE, INTRIN);

    return 0;
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
