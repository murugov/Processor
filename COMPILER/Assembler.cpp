#include "compile.h"
#include "colors.h"

#include "AsmInstrSet.cpp"

// #define DEBUG
#include "DEBUG.h"


AsmErr_t Assembler(FILE *ByteCode, char **arr_cmd, size_t count_cmd) // rename
{    
    if (IS_BAD_PTR(ByteCode))
        return BAD_OUTPUT_FILE_PTR;
        
    ON_DEBUG(
            if (IS_BAD_PTR(arr_cmd))
                return BAD_ARR_CMD_PTR;
            )
        
    byte_t *code = (byte_t*)calloc(count_cmd * (1 + sizeof(arg_t)) + 1, sizeof(byte_t));
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

    sign_t signature = {SIGNATURE, VERSION};

    fwrite(&signature, sizeof(sign_t), 1, ByteCode); 
    fwrite(code, sizeof(byte_t), pc, ByteCode);

    free(code);
    free(arr_labels);
        
    return SUCCESS;
}


AsmErr_t CodeWriter(byte_t *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc, bool is_first_pass)
{
    (*pc) = 0;

    for (size_t line = 0; line < count_cmd; ++line)
    {
        if (strchr(arr_cmd[line], ':') != NULL)
        {
            if (is_first_pass)
            {
                AsmErr_t add_label_verd = AddLabel(arr_labels, arr_cmd[line], count_labels, *pc);

                if (add_label_verd != SUCCESS)
                    return ADD_LABEL_FAIL;
            }
        }

        else
        {
            hash_t hash_func = HashCmd(arr_cmd[line]);

            ssize_t index = 0;
            AsmErr_t search_verd = HashSearch(hash_func, &index);

            if(search_verd != SUCCESS)
                return UNKNOWN_CMD;

            AsmErr_t write_verd = (asm_instr_set[index].func)(code, arr_cmd[line], pc, asm_instr_set[index].cmd, arr_labels, count_labels, is_first_pass);
            if (write_verd == UNKNOWN_LABEL)
            {
                printf(ANSI_COLOR_RED "Unknown label in source.asm:%zu\n" ANSI_COLOR_RESET, line + 1);
                return ERROR;
            }

            else if (write_verd != SUCCESS)
            {
                printf(ANSI_COLOR_RED "Unknown command in source.asm:%zu\n" ANSI_COLOR_RESET, line + 1);
                return ERROR;
            }
        }
    }

    return SUCCESS;
}


AsmErr_t AddLabel(label_t *arr_labels, char* label, size_t *count_labels, size_t pc)
{
    ON_DEBUG(
            if (IS_BAD_PTR(arr_labels))
                return ERROR;
            if (IS_BAD_PTR(label))
                return ERROR;
            if (IS_BAD_PTR(count_labels))
                return ERROR;
            )

    arr_labels[*count_labels].name = label;
    arr_labels[*count_labels].hash = HashCmd(label);
    arr_labels[*count_labels].pc = pc + 3; // одинаковые метки
    (*count_labels)++;

    return SUCCESS;
}


AsmErr_t HashSearch(hash_t hash_func, ssize_t *index)
{
    ON_DEBUG(
            if (IS_BAD_PTR(index))
                return ERROR;
            )
    hash_t *arr_hash = (hash_t*)calloc(LEN_INSTR_SET, sizeof(hash_t));
    for (size_t i = 0; i < LEN_INSTR_SET; ++i)
        arr_hash[i] = asm_instr_set[i].hash;

    *index = BinSearch(arr_hash, LEN_INSTR_SET, hash_func);
    if (*index == -1)
        return UNKNOWN_CMD;
    
    free(arr_hash);

    return SUCCESS;
}