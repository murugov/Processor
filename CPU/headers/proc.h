#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include "stack.h"

#define SIGNATURE "AM"
#define VERSION 1

enum ProcErr_t
{
    TASK_ERROR = 1,
    TASK_SUCCESS = 0,
    BAD_INPUT_FILE_PTR = -1,
    BAD_OUTPUT_FILE_PTR = -2,
    WRONG_FILE_INFO = -3,
    BUFFER_FAIL = -4,
    BAD_ARR_PTR = -5,
    UNKNOWN_CMD = -6,
    WRONG_STK = -7,
    SIGNVERSVER = -8,
    DIV_BY_ZERO = -9,
    STOP_BY_HLT = -10
};

enum InvCmdCode
{
    Inv_cmd_PUSH = 1,
    Inv_cmd_ADD = 2,
    Inv_cmd_SUB = 3,
    Inv_cmd_MUL = 4,
    Inv_cmd_DIV = 5,
    Inv_cmd_POW = 6,
    Inv_cmd_MOD = 7,
    Inv_cmd_SQRT = 8,
    Inv_cmd_OUT = 9,
    Inv_cmd_HLT = 10
};

typedef int cmd_t;
typedef int proc_elem_t;

ProcErr_t Calculator(FILE *stream);
ProcErr_t SignVersVerify(char* buffer);
ProcErr_t CalcExecutor(stk_t *stk, int *buffer, size_t count_cmd);
ProcErr_t CalcFunc(stk_t *stk, int *buffer, size_t *number_cmd);
void CalcErrPrint(ProcErr_t verd);

#endif