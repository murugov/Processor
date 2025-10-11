#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "IsBadPtr.h"

#define SOURCE "./CompileFiles/source.asm"
#define BYTE_CODE "./CompileFiles/bytecode.asm" 

#define SIGNATURE "AM"
#define VERSION 4

#ifdef DEBUG
    #define ON_DEBUG(...)  __VA_ARGS__    // ON_DEBUG ( 
                                          //     if (IsBadPtr (ptr)) return ERROR_BAD_PTR; 
                                          //     if (IsbadPtr (p2)   return ERROR_BAD_P2; 
                                          //     )
#else
    #define ON_DEBUG(...)

#endif    

typedef int cmd_arg_t;
typedef size_t hash_t;

enum AsmErr_t
{
    SUCCESS              = 0x00,
    ERROR                = 0x01,
    BAD_INPUT_FILE_PTR   = 0x02,
    BAD_OUTPUT_FILE_PTR  = 0x03,
    BAD_BUFFER_PTR       = 0x04,
    BAD_ARR_CMD_PTR      = 0x05,
    BAD_CODE_PTR         = 0x06,
    BAD_ARR_LABELS_PTR   = 0x07,
    WRONG_FILE_SIZE      = 0x08,
    CTOR_FAIL            = 0x09,
    CMD_NUM_FAIL         = 0x0A,
    CMD_WITH_ARG_FAIL    = 0x0B,
    CMD_WITHOUT_ARG_FAIL = 0x0C,
    REG_NEX              = 0x0D,
    UNKNOWN_CMD          = 0x0E,
    UNKNOWN_LABEL        = 0x0F
};

enum CmdCodes
{
    CMD_HLT  = 0x00,
    CMD_PUSH = 0x01,
    CMD_POP  = 0x02,
    CMD_ADD  = 0x03,
    CMD_SUB  = 0x04,
    CMD_MUL  = 0x05,
    CMD_DIV  = 0x06,
    CMD_SQRT = 0x07,
    CMD_IN   = 0x08,
    CMD_OUT  = 0x09,
    CMD_JMP  = 0x0A,
    CMD_JE   = 0x0B,
    CMD_JNE  = 0x0C,
    CMD_JA   = 0x0D,
    CMD_JAE  = 0x0E,
    CMD_JB   = 0x0F,
    CMD_JBE  = 0x10,
    CMD_CALL = 0x11,
    CMD_RET  = 0x12
};

enum CmdHash
{
    HASH_HLT  = 0x117D0,
    HASH_PUSH = 0x25A71A,
    HASH_POP  = 0x13631,
    HASH_ADD  = 0xFC81,
    HASH_SUB  = 0x14220,
    HASH_MUL  = 0x12BA4,
    HASH_DIV  = 0x10871,
    HASH_SQRT = 0x26F520,
    HASH_IN   = 0x925,
    HASH_OUT  = 0x1332E,
    HASH_JMP  = 0x11F6D,
    HASH_JE   = 0x93B,
    HASH_JNE  = 0x11F81,
    HASH_JA   = 0x937,
    HASH_JAE  = 0x11DEE,
    HASH_JB   = 0x938,
    HASH_JBE  = 0x11E0D,
    HASH_CALL = 0x1F725E,
    HASH_RET  = 0x13C81
};

#define NUM_REG 16

enum Operands //operands_t
{
    OP_NUM = 0x20,
    OP_REG = 0x40,
    OP_MEM = 0x80
};

struct label_t
{
    const char* name;
    size_t pc;

   // ON_DEBUG ( const char* descr; )
};

AsmErr_t CmdWithArg(char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);
AsmErr_t CmdWithoutArg(char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);
typedef AsmErr_t (*func_t)(char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);

struct WrapCmd
{
    func_t func;
    hash_t hash;
    CmdCodes cmd; 
};


size_t CmdNumber(char* buffer);
AsmErr_t Compiler(FILE *SourceFile, FILE *ByteCode);
AsmErr_t ArrPtrCtor(char *buffer, char **arr_ptr);
AsmErr_t Assembler(char **arr_ptr, size_t count_n, FILE *ByteCode);
AsmErr_t CodeWriter(char *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc);
hash_t HashCmd(const char *buffer);
AsmErr_t HashArrCtor();
void AsmErrPrint(AsmErr_t verd);

#define IS_BAD_PTR(ptr) IsBadPtr((void*)ptr)

#endif