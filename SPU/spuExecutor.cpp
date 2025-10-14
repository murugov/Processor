#include "spu.h"
#include "colors.h"
#include "SizeFile.h"
#include "STACK/StackFunc.hpp"

WrapCmd spu_instr_set[] = 
{
    {FUNC_CMD_HLT, CMD_HLT}, // похожее название
    {FUNC_CMD_PUSH, CMD_PUSH},
    {FUNC_CMD_POP, CMD_POP},
    {FUNC_CMD_ADD, CMD_ADD},
    {FUNC_CMD_SUB, CMD_SUB},
    {FUNC_CMD_MUL, CMD_MUL},
    {FUNC_CMD_DIV, CMD_DIV},
    {FUNC_CMD_SQRT, CMD_SQRT},
    {FUNC_CMD_IN, CMD_IN},
    {FUNC_CMD_OUT, CMD_OUT},
    {FUNC_CMD_JMP, CMD_JMP},
    {FUNC_CMD_JE, CMD_JE},
    {FUNC_CMD_JNE, CMD_JNE},
    {FUNC_CMD_JA, CMD_JA},
    {FUNC_CMD_JAE, CMD_JAE},
    {FUNC_CMD_JB, CMD_JB},
    {FUNC_CMD_JBE, CMD_JBE}, //отдельная функция и потом через define
    {FUNC_CMD_CALL, CMD_CALL},
    {FUNC_CMD_RET, CMD_RET}
};

#define LEN_INSTR_SET sizeof(spu_instr_set) / sizeof(*spu_instr_set)


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
        spuErr_t calc_verd = UNKNOWN_CMD;

        for (size_t wrap = 0; wrap < LEN_INSTR_SET; ++wrap)
        {
            char cmd = (spu->code)[spu->pc] & 0x1F;

            if (spu_instr_set[wrap].cmd == cmd)
            {
                calc_verd = (spu_instr_set[wrap].func)(spu);
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