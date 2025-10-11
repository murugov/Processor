#include "spu.h"
#include "colors.h"


spuErr_t CmdHLT(spu_t * /*spu*/)
{
    return STOP_BY_HLT;
}

spuErr_t CmdPUSH(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

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
    
    if(StackPush(&spu->stk, number_1))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t CmdPOP(spu_t *spu)
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

spuErr_t CmdADD(spu_t *spu)
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

spuErr_t CmdSUB(spu_t *spu)
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

spuErr_t CmdMUL(spu_t *spu)
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

spuErr_t CmdDIV(spu_t *spu)
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

spuErr_t CmdSQRT(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;
    
    if (number_1 < 0)
        return SQRT_NEG;

    if(StackPush(&spu->stk, (stk_elem_t)sqrt(number_1)))
        return ERROR_STK;

    return SUCCESS;
}

spuErr_t CmdIN(spu_t *spu)
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

spuErr_t CmdOUT(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    cmd_arg_t number_1 = 0;

    if(StackPop(&spu->stk, &number_1))
        return ERROR_STK;

    printf(ANSI_COLOR_GREEN "OUT: %d\n" ANSI_COLOR_RESET, number_1);

    return SUCCESS;
}

spuErr_t CmdJumps(spu_t *spu)
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

spuErr_t CmdCALL(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    return SUCCESS;
}

spuErr_t CmdRET(spu_t *spu)
{
    if (IS_BAD_PTR(spu))
        return BAD_SPU_PTR;

    return SUCCESS;
}












// spuErr_t CalcFunc(spu_t *spu, size_t *arr_cmd_pc, size_t count_cmd)
// {
//     cmd_arg_t number_1 = 0;
//     cmd_arg_t number_2 = 0;
//     cmd_arg_t number_3 = 0;

//     char cmd = (spu->code)[spu->pc] & 0x1F;

//     switch (cmd)
//     {
//         case CMD_HLT:
//             return STOP_BY_HLT;

//         case CMD_PUSH:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             }
            
//             // else if (((spu->code)[spu->pc] & 0xE0) == OP_NUM | OP_REG)
//             // {
//             //     (spu->pc)++;
                
//             //     if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//             //         return REG_NEX; // non-existent register

//             //     number_1 = spu->regs[spu->code[spu->pc]];
//             // }    
            
//             if(StackPush(&spu->stk, number_1))
//                 return ERROR_STK;
            
//             break;

//         case CMD_POP:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             (spu->pc)++;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;

//             if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                 return ARG_NEX;

//             spu->regs[(size_t)spu->code[spu->pc]] = number_1;

//             break;

//         //add ALU
//         case CMD_ADD:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;
//             if(StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;

//             if(StackPush(&spu->stk, number_1 + number_2))
//                 return ERROR_STK;

//             break;

//         case CMD_SUB:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;
//             if(StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;

//             if(StackPush(&spu->stk, number_2 - number_1))
//                 return ERROR_STK;

//             break;

//         case CMD_MUL:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;
//             if(StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;

//             if(StackPush(&spu->stk, number_1 * number_2))
//                 return ERROR_STK;

//             break;

//         case CMD_DIV:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;
//             if(StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;

//             if (number_1 != 0)
//             {
//                 if(StackPush(&spu->stk, number_2 / number_1))
//                     return ERROR_STK;
//             }

//             else
//                 return DIV_BY_ZERO;

//             break;

//         case CMD_SQRT:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;
            
//             if (number_1 < 0)
//                 return SQRT_NEG;

//             if(StackPush(&spu->stk, (stk_elem_t)sqrt(number_1)))
//                 return ERROR_STK;
                
//             break;

//         case CMD_IN: //only for PUSH
//             arr_cmd_pc[count_cmd] = spu->pc;

//             printf("Enter command:\n");
//             scanf("%*s %d", &number_1);

//             if(StackPush(&spu->stk, number_1))
//                 return ERROR_STK;

//             break;

//         case CMD_OUT:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if(StackPop(&spu->stk, &number_1))
//                 return ERROR_STK;

//             printf(ANSI_COLOR_GREEN "OUT: %d\n" ANSI_COLOR_RESET, number_1);

//             break;

//         case CMD_JMP:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;
                
//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);

//             spu->pc = arr_cmd_pc[(size_t)number_1] - 1;
            
//             break;

//         case CMD_JE:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK; 

//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d == %d\n", number_2, number_3);
//             if (number_2 == number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         case CMD_JNE:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK; 

//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d != %d\n", number_2, number_3);

//             if (number_2 != number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         case CMD_JA:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             }

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK; 

//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d < %d\n", number_2, number_3);

//             if (number_2 < number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         case CMD_JAE:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK;    
                
//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d <= %d\n", number_2, number_3);

//             if (number_2 <= number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         case CMD_JB:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (cmd_arg_t)(spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK;

//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d > %d\n", number_2, number_3);

//             if (number_2 > number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         case CMD_JBE:
//             arr_cmd_pc[count_cmd] = spu->pc;

//             if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
//             {
//                 for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
//                 {
//                     (spu->pc)++;
//                     number_1 += (spu->code)[spu->pc] * (cmd_arg_t)pow(256, byte);
//                 }
//             }

//             else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
//             {
//                 (spu->pc)++;

//                 if (spu->code[spu->pc] >= NUM_REG || spu->code[spu->pc] < 0)
//                     return ARG_NEX;

//                 number_1 = spu->regs[(size_t)spu->code[spu->pc]];
//             } 

//             if ((size_t)number_1 >= count_cmd || number_1 < 0)
//                 return ARG_NEX;

//             if (StackPop(&spu->stk, &number_2))
//                 return ERROR_STK;
//             if (StackPop(&spu->stk, &number_3))
//                 return ERROR_STK;

//             printf("\narr_cmd_pc[number_1] - 1 = %zu\n", arr_cmd_pc[(size_t)number_1] - 1);
//             printf("%d >= %d\n", number_2, number_3);

//             if (number_2 >= number_3)
//                 spu->pc = arr_cmd_pc[(size_t)number_1] - 1;

//             break;

//         default:
//             return UNKNOWN_CMD;
//     }

//     return SUCCESS;
// }