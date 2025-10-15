#include "spu.h"
#include "colors.h"

spuErr_t ReadArg(spu_t *spu, cmd_arg_t *val)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    if (IS_BAD_PTR(val))
        return BAD_SPU_PTR;

    if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
    { 
        union {
            cmd_arg_t signed_val;
            unsigned char bytes[sizeof(cmd_arg_t)];
        } converter;
        
        for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
        {
            (spu->pc)++;
            converter.bytes[byte] = (unsigned char)(spu->code)[spu->pc];
        }
        
        *val = converter.signed_val;
    }
    
    else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
    {
        (spu->pc)++;
        
        if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
            return ARG_NEX;
        
        *val = spu->regs[(size_t)spu->code[spu->pc]];
    }

    return SUCCESS;
}

#define JWC(spu, condition) \
    do { \
        if (IS_BAD_PTR(spu)) \
            return BAD_SPU_PTR; \
        \
        cmd_arg_t jump_addr = 0; \
        cmd_arg_t val_1 = 0, val_2 = 0; \
        \
        spuErr_t read_arg_verd = ReadArg(spu, &jump_addr); \
        if(read_arg_verd != SUCCESS) \
            return read_arg_verd; \
        \
        if (jump_addr < 0) \
            return ARG_NEX; \
        \
        if (StackPop(&spu->stk, &val_1)) \
            return ERROR_STK; \
        if (StackPop(&spu->stk, &val_2)) \
            return ERROR_STK; \
        \
        if (condition) \
            spu->pc = (size_t)jump_addr - 1; \
    } while(0)


#define POP_value(spu, val_1, val_2, count) \
                        do { \
                            if (IS_BAD_PTR(spu)) \
                                return BAD_SPU_PTR; \
                            \
                            if(StackPop(&spu->stk, &val_1)) \
                                return ERROR_STK; \
                            \
                            if (count == 2) \
                            { \
                            if(StackPop(&spu->stk, &val_2)) \
                                return ERROR_STK; \
                            } \
                            \
                        } while(0)


spuErr_t FUNC_CMD_HLT(spu_t * /*spu*/)
{
    return STOP_BY_HLT;
}

spuErr_t FUNC_CMD_PUSH(spu_t *spu)
{
    cmd_arg_t val_1 = 0;

    spuErr_t read_arg_verd = ReadArg(spu, &val_1);
    if(read_arg_verd != SUCCESS)
        return read_arg_verd;
    
    if(StackPush(&spu->stk, val_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_POP(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t val_1 = 0;

    if(StackPop(&spu->stk, &val_1))
        return ERROR_STK;

    if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
        return ARG_NEX;

    spu->regs[(size_t)spu->code[spu->pc]] = val_1;

    return SUCCESS;
}

spuErr_t FUNC_CMD_ADD(spu_t *spu)
{
    cmd_arg_t val_1 = 0;
    cmd_arg_t val_2 = 0;

    POP_value(spu, val_1, val_2, 2);

    if(StackPush(&spu->stk, val_2 + val_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_SUB(spu_t *spu)
{
    cmd_arg_t val_1 = 0;
    cmd_arg_t val_2 = 0;

    POP_value(spu, val_1, val_2, 2);
    
    if(StackPush(&spu->stk, val_2 - val_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_MUL(spu_t *spu)
{
    cmd_arg_t val_1 = 0;
    cmd_arg_t val_2 = 0;

    POP_value(spu, val_1, val_2, 2);
    
    if(StackPush(&spu->stk, val_2 * val_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_DIV(spu_t *spu)
{
    cmd_arg_t val_1 = 0;
    cmd_arg_t val_2 = 0;

    POP_value(spu, val_1, val_2, 2);

    if (val_1 != 0)
    {
        if(StackPush(&spu->stk, val_2 - val_1))
            return ERROR_STK;

        return SUCCESS;
    }

    return DIV_BY_ZERO;
}

spuErr_t FUNC_CMD_SQRT(spu_t *spu)
{
    cmd_arg_t val_1 = 0;
    cmd_arg_t val_2 = 0;

    POP_value(spu, val_1, val_2, 1);
    
    if (val_1 >= 0)
    {
        if(StackPush(&spu->stk, (cmd_arg_t)sqrt(val_1)))
            return ERROR_STK;
    
        return SUCCESS;
    }

    return SQRT_NEG;
}

spuErr_t FUNC_CMD_IN(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t val_1 = 0;

    printf("Enter command:\n");
    scanf(" %d ", &val_1);

    if(StackPush(&spu->stk, val_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t FUNC_CMD_OUT(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t val_1 = 0;

    if(StackPop(&spu->stk, &val_1))
        return ERROR_STK;

    printf(ANSI_COLOR_GREEN "OUT: %d\n" ANSI_COLOR_RESET, val_1);

    return SUCCESS;
}

spuErr_t FUNC_CMD_JMP(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t jump_addr = 0;

    spuErr_t read_arg_verd = ReadArg(spu, &jump_addr); 
    if(read_arg_verd != SUCCESS)
        return read_arg_verd;

    if (jump_addr < 0)
        return ARG_NEX;

    spu->pc = (size_t)jump_addr - 1;

    return SUCCESS;
}

spuErr_t FUNC_CMD_JE(spu_t *spu)
{
    JWC(spu, val_2 == val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_JNE(spu_t *spu)
{
    JWC(spu, val_2 != val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_JA(spu_t *spu)
{
    JWC(spu, val_2 < val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_JAE(spu_t *spu)
{
    JWC(spu, val_2 <= val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_JB(spu_t *spu)
{
    JWC(spu, val_2 > val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_JBE(spu_t *spu)
{
    JWC(spu, val_2 >= val_1);
    return SUCCESS;
}

spuErr_t FUNC_CMD_CALL(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    if(StackPush(&spu->stk_ret, spu->pc + 1))
        return ERROR_STK;

    cmd_arg_t jump_addr = 0;

    spuErr_t read_arg_verd = ReadArg(spu, &jump_addr); 
    if(read_arg_verd != SUCCESS)
        return read_arg_verd;

    if (jump_addr < 0)
        return ARG_NEX;

    spu->pc = (size_t)jump_addr - 1;

    return SUCCESS;
}

spuErr_t FUNC_CMD_RET(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    size_t jump_addr = 0;

    if(StackPop(&spu->stk_ret, &jump_addr))
        return ERROR_STK;

    if (jump_addr < 0)
        return ARG_NEX;

    spu->pc = jump_addr - 1;
    
    return SUCCESS;
}