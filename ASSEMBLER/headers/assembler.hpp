#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "CONFIG.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "IsBadPtr.hpp"
#include "logger.hpp"
#include "hash_table.hpp"
#include "colors.hpp"

#define SIGNATURE  {'A', 'M'}
#define VERSION    7

#define NUM_REG         16
#define MAX_LEN_CMD     8
#define MAX_LEN_LABEL   8

typedef int           arg_t;
typedef unsigned char count_t;
typedef unsigned char byte_t;


struct line_with_num_t
{
    char   *ptr;
    size_t line;
};

struct sign_t
{
    const char signature[2];
    byte_t     vers;
};


struct label_t
{
    const char* name;
    size_t      pc;
};

struct label_context
{
    label_t*        arr_lables;
    size_t          count_labels;
    ht_t<label_t*>* ht_labels;
};


#include "CmdCodesEnum.hpp"

enum AsmErr_t
{
    ASM_SUCCESS              = 0x00,
    ASM_ERROR                = 0x01,
    ASM_BAD_INPUT_FILE_PTR   = 0x02,
    ASM_BAD_OUTPUT_FILE_PTR  = 0x03,
    ASM_BAD_BUFFER_PTR       = 0x04,
    ASM_BAD_ARR_CMD_PTR      = 0x05,
    ASM_BAD_CODE_PTR         = 0x06,
    ASM_BAD_ARR_LABELS_PTR   = 0x07,
    ASM_BAD_HT_LABELS_PTR    = 0x08,
    ASM_WRONG_FILE_SIZE      = 0x09,
    ASM_CTOR_FAIL            = 0x0A,
    ASM_CMD_NUM_FAIL         = 0x0B,
    ASM_CMD_WITH_ARG_FAIL    = 0x0C,
    ASM_CMD_WITHOUT_ARG_FAIL = 0x0D,
    ASM_LINE_SIZE_EXCEED     = 0x0E,
    ASM_REG_NEX              = 0x0F,
    ASM_MEM_NEX              = 0x10,
    ASM_ARG_NEX              = 0x11,
    ASM_UNKNOWN_CMD          = 0x12,
    ASM_UNKNOWN_LABEL        = 0x13,
    ASM_RE_LABEL             = 0x14
};

enum mask_t
{
    MASK_EMP = 0x00,
    MASK_NUM = 0x01,
    MASK_REG = 0x02,
    MASK_MEM = 0x04,
    MASK_LAB = 0x08
};

enum operands_t
{
    OP_NUM       = 0x20,
    OP_REG       = 0x40,
    OP_MEM       = 0x80,
    OP_TWO_BYTES = 0x80
};


struct asm_context
{
    byte_t*        code;
    char*          ptr;
    byte_t         mask;
    size_t         pc;
    CmdCodes       cmd;
    label_context* lbl_ctx;
    bool           is_first_pass;
};

int LabelComparator(const void *ht_elem, const char *item);
AsmErr_t AddLabel(line_with_num_t *line, label_context *lbl_ctx, size_t pc);
AsmErr_t FindLabel(const char *label_name, ht_t<label_t*> *ht_labels, size_t *pc);
size_t SearchCmdByHash(hash_t hash_cmd);
int CmpForBinSearch(const void *a, const void *b);

AsmErr_t ReadOpcode16(asm_context *write_params);
AsmErr_t ReadOpcode8(asm_context *write_params);
typedef AsmErr_t (*func_t)(asm_context *write_params);

AsmErr_t CheckArg(asm_context *write_params, byte_t count_args);

AsmErr_t ArgIsConstNum(asm_context *write_params, byte_t count_args);
AsmErr_t ArgIsReg(asm_context *write_params, byte_t count_args);
AsmErr_t ArgIsMem(asm_context *write_params, byte_t count_args);
AsmErr_t ArgIsLabel(asm_context *write_params, byte_t count_args);
AsmErr_t LabelSearch(hash_t hash_label, label_t *arr_labels, size_t count_labels, ssize_t *index);


struct wrap_t
{
    func_t   func;
    byte_t   mask;
    hash_t   hash;
    CmdCodes cmd; 
};

AsmErr_t VerifyAsmInstrSetSort();

line_with_num_t* ArrPtrCtor(FILE *SourceFile, char* buffer, size_t *count_lines);
void ArrPtrDtor(char *buffer, line_with_num_t *lines);

void RemoveComments(line_with_num_t *lines, size_t *count_lines);
line_with_num_t* SpecTxtReader(FILE *SourceFile, char* buffer, size_t *count_lines);

AsmErr_t CodeWriter(FILE *ByteCode, line_with_num_t *lines, size_t count_n);
AsmErr_t Compilation(byte_t *code, line_with_num_t *lines, size_t count_lines, label_context *lbl_ctx, size_t *pc);

void AsmErrPrint(char *SourceFile, char *ByteCode, AsmErr_t verd);

#define MASK_CHECK(type_cmd, mask) (type_cmd & mask) == mask

#endif