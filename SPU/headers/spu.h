#ifndef SPU_H
#define SPU_H

#include <math.h>
#include "stack.h"

#define BYTE_CODE "./CompileFiles/bytecode.asm"
#define LOG_FILE "./ReportFiles/LogFile.log"

#define SIGNATURE "AM"
#define VERSION 5

#include "CmdCodesEnums.h"

enum spuErr_t
{
    SUCCESS             = 0,
    ERROR               = 1,
    BAD_INPUT_FILE_PTR  = 2,
    BAD_OUTPUT_FILE_PTR = 3,
    BAD_SPU_PTR         = 4,
    BAD_SPU_CODE_PTR    = 5,
    BAD_SPU_REGS_PTR    = 6,
    BAD_SPU_RAM_PTR     = 7,
    BAD_ARR_PTR         = 8,
    BAD_ARR_CMD_PC_PTR  = 9,
    SPU_DTOR_FAIL       = 10,
    WRONG_FILE_SIZE     = 11,
    WRONG_STK           = 12,
    WRONG_SIGN          = 13,
    WRONG_VERS          = 14,
    UNKNOWN_CMD         = 15,
    DIV_BY_ZERO         = 16,
    SQRT_NEG            = 17,
    ARG_NEX             = 18,
    MEM_NEX             = 19,
    STOP_BY_HLT         = 20,
    ERROR_STK           = 21
};

enum operands_t
{
    OP_NUM = 0x20,
    OP_REG = 0x40,
    OP_MEM = 0x80
};

typedef int arg_t;
typedef size_t hash_t;

#define START_STK_CAP 16
#define NUM_REG 16
#define NUM_RAM 128

struct spu_t
{
    unsigned char *code;
    size_t pc;
    stk_t<arg_t> stk;
    arg_t *regs;
    stk_t<unsigned long> stk_ret;
    arg_t *ram;
};


spuErr_t FUNC_CMD_HLT(spu_t *spu);
spuErr_t FUNC_CMD_PUSH(spu_t *spu);
spuErr_t FUNC_CMD_POP(spu_t *spu);
spuErr_t FUNC_CMD_ADD(spu_t *spu);
spuErr_t FUNC_CMD_SUB(spu_t *spu);
spuErr_t FUNC_CMD_MUL(spu_t *spu);
spuErr_t FUNC_CMD_DIV(spu_t *spu);
spuErr_t FUNC_CMD_SQRT(spu_t *spu);
spuErr_t FUNC_CMD_IN(spu_t *spu);
spuErr_t FUNC_CMD_OUT(spu_t *spu);
spuErr_t FUNC_CMD_JMP(spu_t *spu);
spuErr_t FUNC_CMD_JE(spu_t *spu);
spuErr_t FUNC_CMD_JNE(spu_t *spu);
spuErr_t FUNC_CMD_JA(spu_t *spu);
spuErr_t FUNC_CMD_JAE(spu_t *spu);
spuErr_t FUNC_CMD_JB(spu_t *spu);
spuErr_t FUNC_CMD_JBE(spu_t *spu);
spuErr_t FUNC_CMD_CALL(spu_t *spu);
spuErr_t FUNC_CMD_RET(spu_t *spu);
typedef spuErr_t (*func_t)(spu_t *spu);

struct WrapCmd
{
    func_t func;
    CmdCodes cmd; 
};

spuErr_t SignVersVerify(unsigned char* code);
spuErr_t spuCtor(spu_t *spu, FILE *stream);
spuErr_t spuExecutor(spu_t *spu);
spuErr_t spuDtor(spu_t *spu);
void spuErrPrint(spuErr_t verd);

#endif