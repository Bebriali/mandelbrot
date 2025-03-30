#include "TXLib.h"
#include <stdio.h>
#include <stdlib.h>

const int x_cntr = 500;
const int y_cntr = 300;
const double dx = 0.005;
const double dy = 0.005;
const double MAX_RADIUS = 10;

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
    WHITE_COL
};

double radius(double x, double y);
COLORREF SetPixCol(color pix_col);

int main()
{
    txCreateWindow (800, 600);
    int counter = 0;

    for (int x_i = 0; x_i < 800; x_i++)
    {
        for (int y_i = 0; y_i < 600; y_i++)
        {
            int N = 0;
            double x1 = dx * (double)(x_i - x_cntr);
            double y1 = dx * (double)(y_i - y_cntr);


            double x = 0;
            double y = 0;

            while(radius(x, y) <= MAX_RADIUS && N <= MAX_N)
            {
                x = x * x - y * y + x1;
                y = 2 * x * y + y1;

                N++;
            }

            if (N >= MAX_N) counter++;
            color pix_col;
            if (N <= MAX_N)
            {
                pix_col = BLACK_COL;
            }
            else if (radius(x, y) < MAX_RADIUS / 20)
            {
                pix_col = PINK_COL;
            }
            else
            {
                pix_col = BLUE_COL;
            }


            COLORREF col = SetPixCol(pix_col);
            txSetPixel(x_i, y_i, col);
            // if (x1 < 0)
            // {
            //     txSetPixel(x_i, y_i, RGB(255, 0, 0));
            // }
        }
    }

    printf("counter = %d\n", counter);

    return 0;
}

double radius(double x, double y)
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
            return RGB(  0,   0, 255);
            break;
        case BLACK_COL:
            return RGB(  0,   0,   0);
            break;
        case PINK_COL:
            return RGB(255, 128, 128);
            break;
        case WHITE_COL:
            return RGB(255, 255, 255);
            break;
        default:
            printf("error in color definition\n");
            return RGB(0, 0, 0);
    }

    return RGB(0, 0, 0);
}
