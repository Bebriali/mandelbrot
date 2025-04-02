#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <scan_code.h>
#include <time.h>

const long double SCALE = 0.005;
const int MAX_RADIUS    = 20;
      int x_cntr        = 500;
      int y_cntr        = 300;
const int WIDTH         = 800;
const int HEIGHT        = 600;
const int STEP          = 10;

enum limit_conditions
{
    MAX_N = 1000
};

enum color
{
    RED_COL,
    BLUE_COL,
    BLACK_COL,
    PINK_COL,
    WHITE_COL,
    YELLOW_COL
};

void HandleArgs(char** argv, bool* show_flag, int* eval_flag);
long double radius(long double x, long double y);
COLORREF SetPixCol(color pix_col);
void CountAndRender(bool show, int evaluate, scan_code_t nxt_instr);

int main(int argc, char* argv[])
{
    bool SHOW = true;
    int  EVALUATE = 1;
    if (argc > 1)
    {
        if (argc > 2)
        {
            HandleArgs(argv, &SHOW, &EVALUATE);
        }
        else
        {
            HandleArgs(argv, &SHOW, NULL);
        }
    }

    //if (SHOW)

    txCreateWindow (WIDTH, HEIGHT);
    while (true)
    {
        scan_code_t nxt_instr = GetKey();
        if (nxt_instr == ESC_C) break;

        CountAndRender(SHOW, EVALUATE, nxt_instr);
    }

    printf("sizeof color = %d\n", sizeof(color));


    return 0;
}


void CountAndRender(bool show, int evaluate, scan_code_t nxt_instr)
{
    switch (nxt_instr)
    {
        case UP_C:
            y_cntr -= STEP;
            break;
        case DOWN_C:
            y_cntr += STEP;
            break;
        case LEFT_C:
            x_cntr -= STEP;
            break;
        case RIGHT_C:
            x_cntr += STEP;
            break;
        default:
            break;
    }

    for (int x_i = 0; x_i < WIDTH; x_i++)
    {
        for (int y_i = 0; y_i < HEIGHT; y_i++)
        {
            for (int i = 0; i < evaluate; i++)
            {
                int N = 0;
                long double x1 = SCALE * (long double)(x_i - x_cntr);
                long double y1 = SCALE * (long double)(y_i - y_cntr);


                long double x = 0;
                long double y = 0;

                long double RAD = radius(x, y);

                while(RAD <= MAX_RADIUS && N <= MAX_N)
                {
                    long double x_2 = x * x;
                    long double y_2 = y * y;
                    long double xy  = x * y;

                    x = x_2 - y_2 + x1;
                    y = xy  +  xy + y1;
                    RAD = radius(x, y);

                    N++;
                }

                if (show)
                {
                    color pix_col;

                    if (N <= MAX_N)
                    {
                        if (N <= 30)
                            pix_col = BLACK_COL;
                        else if (N > 30 && N <= 40)
                            pix_col = PINK_COL;
                        else
                            pix_col = RED_COL;
                    }
                    else
                    {
                        // if (RAD < 0.0001 * MAX_RADIUS)
                            pix_col = BLUE_COL;
                        // else if (RAD >= 0.0001 * MAX_RADIUS && RAD < 0.0005 * MAX_RADIUS)
                        //     pix_col = WHITE_COL;
                        // else
                        //     pix_col = YELLOW_COL;
                    }

                    COLORREF col = SetPixCol(pix_col);
                    txSetPixel(x_i, y_i, col);
                }
            }
        }
    }

    return ;
}

void HandleArgs(char** argv, bool* show_flag, int* eval_flag)
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
                return ;
            }
        }
    }

    return ;
}

long double radius(long double x, long double y)
{
    return x * x + y * y;
}

COLORREF SetPixCol(color pix_col)
{
    switch(pix_col)
    {
        case RED_COL:
            return RGB(255,   0,   0);
            break;
        case BLUE_COL:
            return RGB( 42,  160, 255);
            break;
        case BLACK_COL:
            return RGB(  0,   0,   0);
            break;
        case PINK_COL:
            return RGB(255, 128, 128);
            break;
        case WHITE_COL:
            return RGB(24, 174, 255);
            break;
        case YELLOW_COL:
            return RGB(0, 164,   228);
        default:
            printf("error in color definition\n");
            return RGB(0, 0, 0);
    }

    return RGB(0, 0, 0);
}
