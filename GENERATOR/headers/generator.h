#ifndef GENERATOR_H
#define GENERATOR_H

#define MAX_LEN_NAME_CMD 8
#define MAX_LEN_NAME_SPEC 16

typedef char* spec_t;
typedef size_t hash_t;
typedef char* cmd_t;
typedef unsigned int num_t;

enum GenErr_t
{
    SUCCESS = 0,
    ERROR   = 1
};

enum WithHash
{
    WITH    = 0x28A8C6,
    WITHOUT = 0x7B918CA8
};

struct asm_instr_t
{
    spec_t spec;
    hash_t hash;
    cmd_t  cmd;
};

struct spu_instr_t
{
    num_t num;
    cmd_t cmd;
};

GenErr_t GenCmdEnum(FILE *CmdEnumsFile, char **arr_ptr, size_t count_line);
GenErr_t GenAsmInstrSet(FILE *AsmInstrSetFile, char **arr_ptr, size_t count_line);
GenErr_t GenSpuInstrSet(FILE *SpuInstrSetFile, char **arr_ptr, size_t count_line);
hash_t HashCmd(const char *cmd);
int CmpByHash(const void *a, const void *b);
int CmpByNum(const void *a, const void *b);

#endif