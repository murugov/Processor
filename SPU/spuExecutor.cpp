#include "spu.h"
#include "colors.h"
#include "SizeFile.h"

#include "SpuInstrSet.cpp"


spuErr_t spuCtor(spu_t *spu, FILE *stream)
{
    if (IS_BAD_PTR(stream))
        return BAD_INPUT_FILE_PTR;

    ssize_t file_size = SizeFile(stream);
    if (file_size < 0)
        return WRONG_FILE_SIZE;

    spu->code = (unsigned char*)calloc((size_t)file_size + 1, sizeof(unsigned char));
    if (IS_BAD_PTR(spu->code))
        return BAD_SPU_CODE_PTR;

    fread(spu->code, sizeof(unsigned char), (size_t)file_size, stream);

    spuErr_t file_ext_verd = SignVersVerify(spu->code);
    if (file_ext_verd)
        return file_ext_verd;

    spu->pc = (size_t)file_size;

    STACK_INIT(&spu->stk, 16);

    if (spu->stk.error != SUCCESS)
        return WRONG_STK;

    spu->regs = (arg_t*)calloc(NUM_REG, sizeof(arg_t));
    if (IS_BAD_PTR(spu->regs))
        return BAD_SPU_REGS_PTR;

    STACK_INIT(&spu->stk_ret, 16);

    spu->ram = (arg_t*)calloc(NUM_RAM, sizeof(arg_t));
    if (IS_BAD_PTR(spu->ram))
        return BAD_SPU_RAM_PTR;

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
            unsigned char cmd = (spu->code[spu->pc]) & 0x1F;

            if (spu_instr_set[wrap].cmd == cmd)
            {
                calc_verd = (spu_instr_set[wrap].func)(spu);
                if (calc_verd != SUCCESS)
                    return calc_verd;
                    
                break;
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
    StackDtor(&spu->stk); // можно написать доп проверку на dtor
    free(spu->regs);
    StackDtor(&spu->stk_ret);
    HashArrDtor();

    return SUCCESS;
}


spuErr_t SignVersVerify(unsigned char* code)
{
    const char* signature = SIGNATURE;
    unsigned char version = VERSION;

    if (signature[0] != code[0] || signature[1] != code[1])
        return WRONG_SIGN;
    if (version != code[2])
        return WRONG_VERS;

    return SUCCESS;
}