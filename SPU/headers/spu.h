#ifndef SPU_H
#define SPU_H

#include <math.h>
#include "stack.h"

#define BYTE_CODE "./CompileFiles/bytecode.asm"
#define LOG_FILE "./ReportFiles/LogFile.log"

#define SIGNATURE "AM"
#define VERSION 4

enum spuErr_t
{
    SUCCESS             = 0,
    ERROR               = 1,
    BAD_INPUT_FILE_PTR  = 2,
    BAD_OUTPUT_FILE_PTR = 3,
    BAD_SPU_PTR         = 18,
    BAD_SPU_CODE_PTR    = 4,
    BAD_SPU_REGS_PTR    = 17,
    BAD_ARR_PTR         = 5,
    BAD_ARR_CMD_PC_PTR  = 6,
    SPU_DTOR_FAIL       = 19,
    WRONG_FILE_SIZE     = 7,
    WRONG_STK           = 8,
    WRONG_SIGN          = 9,
    WRONG_VERS          = 10,
    UNKNOWN_CMD         = 11,
    DIV_BY_ZERO         = 12,
    SQRT_NEG            = 13,
    ARG_NEX             = 14,
    STOP_BY_HLT         = 15,
    ERROR_STK           = 16
};

enum CmdCodes //это будет в отдельном файле, когда 
{
    CMD_HLT = 0x00,
    CMD_PUSH = 0x01,
    CMD_POP = 0x02,
    CMD_ADD = 0x03,
    CMD_SUB = 0x04,
    CMD_MUL = 0x05,
    CMD_DIV = 0x06,
    CMD_SQRT = 0x07,
    CMD_IN = 0x08,
    CMD_OUT = 0x09,
    CMD_JMP = 0x0A,
    CMD_JE = 0x0B,
    CMD_JNE = 0x0C,
    CMD_JA = 0x0D,
    CMD_JAE = 0x0E,
    CMD_JB = 0x0F,
    CMD_JBE = 0x10,
    CMD_CALL = 0x11,
    CMD_RET = 0x12
};

enum Operands
{
    OP_NUM = 0x20,
    OP_REG = 0x40,
    OP_MEM = 0x80
};

typedef int cmd_arg_t;
typedef size_t hash_t;


struct spu_t
{
    char *code;
    size_t pc;
    stk_t<int> stk;
    cmd_arg_t *regs;
    //stk_t<size_t> stk_ret;
};

#define NUM_REG 16

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

spuErr_t SignVersVerify(char* code);
spuErr_t spuCtor(spu_t *spu, FILE *stream);
spuErr_t spuExecutor(spu_t *spu);
spuErr_t spuDtor(spu_t *spu);
void spuErrPrint(spuErr_t verd);

#endif