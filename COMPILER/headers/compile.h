#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "IsBadPtr.h"

#define SIGNATURE "AM"
#define VERSION 5

typedef int arg_t;
typedef size_t hash_t;

#include "CmdCodesEnums.h"

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
    MEM_NEX              = 0x0E,
    UNKNOWN_CMD          = 0x0F,
    UNKNOWN_LABEL        = 0x10
};

#define NUM_REG 16

enum operands_t
{
    OP_NUM = 0x20,
    OP_REG = 0x40,
    OP_MEM = 0x80
};

struct label_t
{
    const char* name;
    size_t pc;
};


// struct asm_context
// {
//     unsigned char *code;
//     char **arr_ptr; 
//     size_t count_cmd;
//     char *ptr;
//     size_t pc;
//     CmdCodes cmd;
//     label_t *arr_labels;
//     size_t count_labels;
//     bool is_first_pass;
// };

// AsmErr_t CmdWithArg(asm_context* write_params);
// AsmErr_t CmdWithoutArg(asm_context* write_params);
// typedef AsmErr_t (*func_t)(asm_context* write_params);

AsmErr_t CmdWithArg(unsigned char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);
AsmErr_t CmdWithoutArg(unsigned char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);
typedef AsmErr_t (*func_t)(unsigned char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass);

struct WrapCmd
{
    func_t func;
    hash_t hash;
    CmdCodes cmd; 
};


char** ArrPtrCtor(FILE *SourceFile, size_t *count_line);
AsmErr_t Assembler(FILE *ByteCode, char **arr_ptr, size_t count_n);
AsmErr_t CodeWriter(unsigned char *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc, bool is_first_pass);
hash_t HashCmd(const char *buffer);
AsmErr_t HashArrCtor();
void AsmErrPrint(FILE *SourceFile, FILE *ByteCode, AsmErr_t verd);

#define IS_BAD_PTR(ptr) IsBadPtr((void*)ptr)

#endif