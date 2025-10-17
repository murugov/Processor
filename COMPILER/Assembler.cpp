#include "compile.h"
#include "colors.h"

#include "AsmInstrSet.cpp"

// #define DEBUG
#include "DEBUG.h"


AsmErr_t Assembler(FILE *ByteCode, char **arr_cmd, size_t count_cmd)
{    
    if (IS_BAD_PTR(ByteCode))
        return BAD_OUTPUT_FILE_PTR;
        
    ON_DEBUG(
            if (IS_BAD_PTR(arr_cmd))
                return BAD_ARR_CMD_PTR;
            )

        
    unsigned char *code = (unsigned char*)calloc(count_cmd * (1 + sizeof(arg_t)) + 1, sizeof(unsigned char));
    if (IS_BAD_PTR(code))
        return BAD_CODE_PTR;

    size_t count_labels = 0;
    label_t *arr_labels = (label_t*)calloc(count_cmd, sizeof(label_t));
    if (IS_BAD_PTR(arr_labels))
        return BAD_ARR_LABELS_PTR;

    size_t pc = 0;
    
    AsmErr_t first_pass = CodeWriter(code, arr_cmd, count_cmd, arr_labels, &count_labels, &pc, true); // struct asm_context
    if (first_pass != SUCCESS)
        return first_pass;

    AsmErr_t second_pass = CodeWriter(code, arr_cmd, count_cmd, arr_labels, &count_labels, &pc, false);
    if (second_pass != SUCCESS)
        return second_pass;

    const char* signature = SIGNATURE;
    short version = VERSION;

    fwrite(signature, 2, sizeof(char), ByteCode);
    fwrite(&version, 1, sizeof(char), ByteCode);
    fwrite(code, pc, sizeof(unsigned char), ByteCode);

    free(code);
    free(arr_labels);
        
    return SUCCESS;
}


AsmErr_t CodeWriter(unsigned char *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc, bool is_first_pass)
{
    (*pc) = 0;

    for (size_t line = 0; line < count_cmd; ++line)
    {
        if (strchr(arr_cmd[line], ':') != NULL)
        {
            if (is_first_pass)
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
                    //asm_context write_params = {code, arr_cmd, count_cmd, arr_cmd[line], *pc, asm_instr_set[wrap].cmd, arr_labels, *count_labels, is_first_pass};
                    // write_verd = (asm_instr_set[wrap].func)(&write_params);

                    write_verd = (asm_instr_set[wrap].func)(code, arr_cmd[line], pc, asm_instr_set[wrap].cmd, arr_labels, count_labels, is_first_pass);
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

    return SUCCESS;
}