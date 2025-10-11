#include "spu.h"
#include "colors.h"
#include "SizeFile.h"


WrapCmd instr_set[] = 
{
    {CmdHLT, CMD_HLT},
    {CmdPUSH, CMD_PUSH},
    {CmdPOP, CMD_POP},
    {CmdADD, CMD_ADD},
    {CmdSUB, CMD_SUB},
    {CmdMUL, CMD_MUL},
    {CmdDIV, CMD_DIV},
    {CmdSQRT, CMD_SQRT},
    {CmdIN, CMD_IN},
    {CmdOUT, CMD_OUT},
    {CmdJumps, CMD_JMP},
    {CmdJumps, CMD_JE},
    {CmdJumps, CMD_JNE},
    {CmdJumps, CMD_JA},
    {CmdJumps, CMD_JAE},
    {CmdJumps, CMD_JB},
    {CmdJumps, CMD_JBE},
    {CmdCALL, CMD_CALL},
    {CmdRET, CMD_RET}
};

#define LEN_INSTR_SET sizeof(instr_set) / sizeof(*instr_set)


spuErr_t spuCtor(spu_t *spu, FILE *stream)
{
    if (IS_BAD_PTR(stream))
        return BAD_INPUT_FILE_PTR;

    ssize_t file_size = SizeFile(stream);
    if (file_size < 0)
        return WRONG_FILE_SIZE;

    spu->code = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IS_BAD_PTR(spu->code))
        return BAD_SPU_CODE_PTR;

    fread(spu->code, sizeof(char), (size_t)file_size, stream);

    spuErr_t file_ext_verd = SignVersVerify(spu->code);
    if (file_ext_verd)
        return file_ext_verd;

    spu->pc = (size_t)file_size;

    STACK_INIT(&spu->stk, 32);

    if (spu->stk.error != SUCCESS)
        return WRONG_STK;

    spu->regs = (cmd_arg_t*)calloc(NUM_REG, sizeof(cmd_arg_t));
    if (IS_BAD_PTR(spu->regs))
        return BAD_SPU_REGS_PTR;

    return SUCCESS;
}


spuErr_t spuExecutor(spu_t *spu)
{
    size_t count_cmd = spu->pc;
    spu->pc = 3;

    for (;spu->pc < count_cmd; ++(spu->pc))
    {
        //printf();
        spuErr_t calc_verd = UNKNOWN_CMD;

        for (size_t wrap = 0; wrap < LEN_INSTR_SET; ++wrap)
        {
            char cmd = (spu->code)[spu->pc] & 0x1F;

            if (instr_set[wrap].cmd == cmd)
            {
                calc_verd = (instr_set[wrap].func)(spu);
                if (calc_verd != SUCCESS)
                    return calc_verd;
            }
        }

        if (calc_verd == UNKNOWN_CMD)
        {
            printf(ANSI_COLOR_RED "Unknown command\n" ANSI_COLOR_RESET); // return file and number of elem
            return ERROR;
        }
    }

    return SUCCESS;
}


spuErr_t spuDtor(spu_t *spu)
{
    free(spu->code);
    StackDtor(&spu->stk);
    free(spu->regs);

    return SUCCESS;
}


spuErr_t SignVersVerify(char* code)
{
    const char* signature = SIGNATURE;
    char version = VERSION;

    if (signature[0] != code[0] || signature[1] != code[1])
        return WRONG_SIGN;
    if (version != code[2])
        return WRONG_VERS;

    return SUCCESS;
}