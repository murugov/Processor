#ifndef GENERATOR_H
#define GENERATOR_H

typedef size_t hash_t;

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

GenErr_t GenCmdEnum(FILE *SourceFile, FILE *CmdEnumsFile, char **arr_ptr, size_t count_line);
GenErr_t GenAsmInstrSet(FILE *SourceFile, FILE *AsmInstrSetFile, char **arr_ptr, size_t count_line);
GenErr_t GenSpuInstrSet(FILE *SourceFile, FILE *SpuInstrSetFile, char **arr_ptr, size_t count_line);
hash_t HashCmd(const char *cmd);

#endif