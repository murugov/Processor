#ifndef CALC_H
#define CALC_H

#include <stdio.h>

enum Fails 
{
    SUCCESS = 1,
    OPEN_FAIL = 2,
    OUT_FAIL = 4,
    FILE_INFO_FAIL = 8,
    BUFFER_FAIL = 16,
    ARRAY_COMMANDS_FAIL = 32
};

typedef int res_t;

struct commands {
    const char* ptr;
    size_t len;
};

#endif