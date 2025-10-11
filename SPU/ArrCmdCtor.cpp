#include "spu.h"
#include "colors.h"


spuErr_t CmdCmp(spu_t *spu, size_t *arr_cmd_pc, size_t count_cmd)
{
    char cmd = (spu->code)[spu->pc] & 0x1F;

    switch (cmd)
    {
        case CMD_HLT:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_PUSH:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;
    
            break;

        case CMD_POP:
            arr_cmd_pc[count_cmd] = spu->pc;
            (spu->pc)++;
            break;

        case CMD_ADD:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_SUB:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_MUL:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_DIV:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_SQRT:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_IN:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_OUT:
            arr_cmd_pc[count_cmd] = spu->pc;
            break;

        case CMD_JMP:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JE:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JNE:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JA:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JAE:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JB:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        case CMD_JBE:
            arr_cmd_pc[count_cmd] = spu->pc;

            if (((spu->code)[spu->pc] & 0xE0) == OP_NUM)
                spu->pc += sizeof(cmd_arg_t);

            else if (((spu->code)[spu->pc] & 0xE0) == OP_REG)
                (spu->pc)++;

            break;

        default:
            return UNKNOWN_CMD;
    }

    return SUCCESS;
}

spuErr_t ArrCmdCtor(spu_t *spu, size_t *arr_cmd_pc, size_t *count_cmd)
{
    size_t count_code = spu->pc;
    spu->pc = 3;

    for (; spu->pc < count_code; ++(*count_cmd), ++(spu->pc))
    {
        spuErr_t arr_cmd_verd = CmdCmp(spu, arr_cmd_pc, *count_cmd);
        if (arr_cmd_verd != SUCCESS)
        {
            return arr_cmd_verd;
        }
    }

    spu->pc = 3;

    return SUCCESS;
}