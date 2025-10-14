#include "compile.h"
#include "colors.h"

#include "AsmInstrSet.cpp"

//union
// const WrapCmd instr_set[] = 
// {
//     {CmdWithoutArg, HASH_HLT,  CMD_HLT},
//     {CmdWithArg,    HASH_PUSH, CMD_PUSH},
//     {CmdWithArg,    HASH_POP,  CMD_POP},
//     {CmdWithoutArg, HASH_ADD,  CMD_ADD},
//     {CmdWithoutArg, HASH_SUB,  CMD_SUB},
//     {CmdWithoutArg, HASH_MUL,  CMD_MUL},
//     {CmdWithoutArg, HASH_DIV,  CMD_DIV},
//     {CmdWithoutArg, HASH_SQRT, CMD_SQRT},
//     {CmdWithoutArg, HASH_IN,   CMD_IN},
//     {CmdWithoutArg, HASH_OUT,  CMD_OUT},
//     {CmdWithArg,    HASH_JMP,  CMD_JMP},
//     {CmdWithArg,    HASH_JE,   CMD_JE},
//     {CmdWithArg,    HASH_JNE,  CMD_JNE},
//     {CmdWithArg,    HASH_JA,   CMD_JA},
//     {CmdWithArg,    HASH_JAE,  CMD_JAE},
//     {CmdWithArg,    HASH_JB,   CMD_JB},
//     {CmdWithArg,    HASH_JBE,  CMD_JBE},
//     {CmdWithArg,    HASH_CALL, CMD_CALL},
//     {CmdWithoutArg, HASH_RET,  CMD_RET}
// };

// #define LEN_INSTR_SET sizeof(asm_instr_set) / sizeof(*asm_instr_set)


AsmErr_t Assembler(char **arr_cmd, size_t count_cmd, FILE *ByteCode)
{
    char *code = (char*)calloc(count_cmd * (1 + sizeof(cmd_arg_t)) + 1, sizeof(char));
    if (IS_BAD_PTR(code))
        return BAD_CODE_PTR;

    size_t count_labels = 0;
    label_t *arr_labels = (label_t*)calloc(count_cmd, sizeof(label_t));
    if (IS_BAD_PTR(arr_labels))
        return BAD_ARR_LABELS_PTR;

    size_t pc = 0;

    AsmErr_t first_pass = CodeWriter(code, arr_cmd, count_cmd, arr_labels, &count_labels, &pc); // struct asm_context
    if (first_pass != SUCCESS)
        return first_pass;

    AsmErr_t second_pass = CodeWriter(code, arr_cmd, count_cmd, arr_labels, &count_labels, &pc);
    if (second_pass != SUCCESS)
        return second_pass;

    const char* signature = SIGNATURE;
    short version = VERSION;

    fwrite(signature, 2, sizeof(char), ByteCode);
    fwrite(&version, 1, sizeof(char), ByteCode);
    fwrite(code, pc, sizeof(char), ByteCode);

    free(code);
    free(arr_labels);
        
    return SUCCESS;
}


AsmErr_t CodeWriter(char *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc)
{
    static size_t count_pass = 0;//убрать
    (*pc) = 0;

    for (size_t line = 0; line < count_cmd; ++line)
    {
        if (strchr(arr_cmd[line], ':') != NULL)
        {
            if (!count_pass)
            {
                arr_labels[*count_labels].name = arr_cmd[line];
                arr_labels[*count_labels].pc = *pc + 3;
                (*count_labels)++;
            }
        }

        else
        {
            hash_t hash_func = HashCmd(arr_cmd[line]);
            
            AsmErr_t write_verd = UNKNOWN_CMD;

            for (size_t wrap = 0; wrap < LEN_INSTR_SET; ++wrap)
            {
                if (asm_instr_set[wrap].hash == hash_func)
                {
                    write_verd = (asm_instr_set[wrap].func)(code, arr_cmd[line], pc, asm_instr_set[wrap].cmd, arr_labels, count_labels, !count_pass);
                    if (write_verd != SUCCESS)
                    {
                        printf(ANSI_COLOR_RED "Unknown command in source.asm:%zu\n" ANSI_COLOR_RESET, line + 1); // return file unknown label
                        return ERROR;
                    }
                    break;
                }
            }

            if (write_verd == UNKNOWN_CMD)
            {
                printf(ANSI_COLOR_RED "Unknown command in source.asm:%zu\n" ANSI_COLOR_RESET, line + 1); // return file unknown label
                return ERROR;
            }
        }
    }

    count_pass++;

    return SUCCESS;
}