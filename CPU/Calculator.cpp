#include <math.h>
#include "stack.h"
#include "proc.h"
#include "colors.h"
#include "SizeFile.h"

ProcErr_t Calculator(FILE *stream)
{
    stk_t stk = {};
    STACK_INIT(&stk, 8);

    if (stk.error != SUCCESS)
        return WRONG_STK;

    if (IS_BAD_PTR(stream))
        return BAD_INPUT_FILE_PTR;

    ssize_t file_size = SizeFile(stream);
    if (file_size < 0)
        return WRONG_FILE_INFO;

    proc_elem_t* buffer = (proc_elem_t*)calloc((size_t)file_size / 4 + 1, sizeof(proc_elem_t));
    if (IS_BAD_PTR(buffer))
        return BUFFER_FAIL;

    size_t count_cmd = fread(buffer, sizeof(int), (size_t)file_size, stream);
    if (SignVersVerify((char*)buffer))
        return SIGNVERSVER;

    if (CalcExecutor(&stk, buffer, count_cmd))
    {
        free(buffer);
        StackDtor(&stk);
        return TASK_ERROR;
    }

    free(buffer);
    StackDtor(&stk);

    return TASK_SUCCESS;
}

ProcErr_t CalcExecutor(stk_t *stk, int *buffer, size_t count_cmd)
{
    for (size_t number_cmd = 1; number_cmd < count_cmd; ++number_cmd)
    {
        ProcErr_t calc_verd = CalcFunc(stk, buffer, &number_cmd);
        if (calc_verd == STOP_BY_HLT)
            return TASK_SUCCESS;

        if (calc_verd == UNKNOWN_CMD)
            return UNKNOWN_CMD;
    }
 
    return TASK_SUCCESS;
}

ProcErr_t CalcFunc(stk_t *stk, int *buffer, size_t *number_cmd)
{
    proc_elem_t number_1 = 0;
    proc_elem_t number_2 = 0;

    switch (buffer[*number_cmd])
    {
        case Inv_cmd_PUSH:
            StackPush(stk, buffer[*number_cmd + 1]);

            (*number_cmd)++;
            break;

        case Inv_cmd_ADD:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            StackPush(stk, number_1 + number_2);
            break;

        case Inv_cmd_SUB:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            StackPush(stk, number_2 - number_1);
            break;

        case Inv_cmd_MUL:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            StackPush(stk, number_1 * number_2);
            break;

        case Inv_cmd_DIV:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            if (number_1 != 0)
                StackPush(stk, number_2 / number_1);

            else
                return DIV_BY_ZERO;
            break;

        case Inv_cmd_POW:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            StackPush(stk, (stk_elem_t)pow(number_2, number_1));
            break;

        case Inv_cmd_MOD:
            StackPop(stk, &number_1);
            StackPop(stk, &number_2);

            StackPush(stk, number_2 % number_1);
            break;

        case Inv_cmd_SQRT:
            StackPop(stk, &number_1);

            StackPush(stk, (stk_elem_t)sqrt(number_1));
            break;

        case Inv_cmd_OUT:
            StackPop(stk, &number_1);

            printf(ANSI_COLOR_GREEN "OUT: %d\n" ANSI_COLOR_RESET, number_1);
            break;

        case Inv_cmd_HLT:
            return STOP_BY_HLT;

        default:
            return UNKNOWN_CMD;
    }

    return TASK_SUCCESS;
}

ProcErr_t SignVersVerify(char* buffer)
{
    const char* signature = SIGNATURE;
    short version = VERSION;
    if (signature[0] != buffer[0] || signature[1] != buffer[1] || version != (buffer[2] + buffer[3] * 64))
        return TASK_ERROR;

    return TASK_SUCCESS;
}