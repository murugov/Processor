#include "spu.h"
#include "colors.h"


spuErr_t FUNC_CMD_HLT(spu_t * /*spu*/)
{
    return STOP_BY_HLT;
}

spuErr_t FUNC_CMD_PUSH(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
    {
        for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
        {
            (spu->pc)++;
            number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte); //kill pow
        }
    }

    else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
    {
        (spu->pc)++;

        if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
            return ARG_NEX;

        number_1 = spu->regs[(size_t)spu->code[spu->pc]];
    }  
    
    if(StackPush(&spu->stk, number_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_POP(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;

    if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
        return ARG_NEX;

    spu->regs[(size_t)spu->code[spu->pc]] = number_1;

    return SUCCESS;
}

spuErr_t FUNC_CMD_ADD(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;
    cmd_arg_t number_2 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    if(StackPop(&spu->stk, &number_2))
        return ERROR_STK;

    if(StackPush(&spu->stk, number_1 + number_2))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_SUB(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;
    cmd_arg_t number_2 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    if(StackPop(&spu->stk, &number_2))
        return ERROR_STK;

    if(StackPush(&spu->stk, number_2 - number_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_MUL(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;
    cmd_arg_t number_2 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    if(StackPop(&spu->stk, &number_2))
        return ERROR_STK;

    if(StackPush(&spu->stk, number_1 * number_2))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_DIV(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;
    cmd_arg_t number_2 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    if(StackPop(&spu->stk, &number_2))
        return ERROR_STK;

    if (number_1 != 0)
    {
        if(StackPush(&spu->stk, number_2 / number_1))
            return ERROR_STK;
    }

    else
        return DIV_BY_ZERO;

    return SUCCESS;
}

spuErr_t FUNC_CMD_SQRT(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    
    if (number_1 < 0)
        return SQRT_NEG;

    if(StackPush(&spu->stk, (cmd_arg_t)sqrt(number_1)))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_IN(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    printf("Enter command:\n");
    scanf(" %d ", &number_1);

    if(StackPush(&spu->stk, number_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_OUT(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;

    printf(ANSI_COLOR_GREEN "OUT: %d\n" ANSI_COLOR_RESET, number_1);

    return SUCCESS;
}

spuErr_t FUNC_CMD_JMP(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;
    cmd_arg_t number_2 = 0;
    cmd_arg_t number_3 = 0;

    if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
    {
        for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
        {
            (spu->pc)++;
            number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
        }
    }

    else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
    {
        (spu->pc)++;

        if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
            return ARG_NEX;

        number_1 = spu->regs[(size_t)spu->code[spu->pc]];
    } 

    if (number_1 < 0) //(size_t)number_1 >= count_cmd || 
        return ARG_NEX;
        
    spu->pc = (size_t)number_1 - 1;

    return SUCCESS;
}

spuErr_t FUNC_CMD_CALL(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    return SUCCESS;
}

spuErr_t FUNC_CMD_RET(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    return SUCCESS;
}