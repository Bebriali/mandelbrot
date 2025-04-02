#ifndef SCAN_CODE_H
#define SCAN_CODE_H

#include <stdio.h>
#include <conio.h>

enum scan_code_t
{
    PREF_C_1 = 0,
    PREF_C_2 = 240,
    ESC_C    = 27,
    UP_C     = 119,
    LEFT_C   = 97,
    RIGHT_C  = 100,
    DOWN_C   = 115,
    ERROR_C  = 1000
};

scan_code_t GetKey(void);

#endif
