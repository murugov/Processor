#include "compile.h"
#include "colors.h"

#include "AsmInstrSet.cpp"

// #define DEBUG
#include "DEBUG.h"


AsmErr_t SecondCompilation(byte_t *code, char **arr_cmd, size_t count_cmd, label_t *arr_labels, size_t *count_labels, size_t *pc)
{
    ON_DEBUG(
            if (IS_BAD_PTR(code))
                return ERROR;
            if (IS_BAD_PTR(arr_cmd))
                return ERROR;
            if (IS_BAD_PTR(arr_labels))
                return ERROR;
            if (IS_BAD_PTR(count_labels))
                return ERROR;
            if (IS_BAD_PTR(pc))
                return ERROR;
    )

    (*pc) = 0;
    asm_context write_params = {code, NULL, *pc, asm_instr_set[0].cmd, arr_labels, *count_labels};

    for (size_t line = 0; line < count_cmd; ++line)
    {
        if (strchr(arr_cmd[line], ':') != NULL)
            continue;

        hash_t hash_func = HashCmd(arr_cmd[line]);

        ssize_t index = 0;
        AsmErr_t search_verd = HashSearch(hash_func, &index);

        if(search_verd != SUCCESS)
            return search_verd;

        write_params.ptr = arr_cmd[line];
        write_params.cmd = asm_instr_set[index].cmd;
        AsmErr_t write_verd = (asm_instr_set[index].func)(&write_params);

        if (write_verd != SUCCESS)
            return write_verd;
    }

    code = write_params.code;
    *pc = write_params.pc;
    arr_labels = write_params.arr_labels;
    *count_labels = write_params.count_labels;

    return SUCCESS;
}


AsmErr_t HashSearch(hash_t hash_func, ssize_t *index)
{
    ON_DEBUG(
            if (IS_BAD_PTR(index))
                return ERROR;
            )

    hash_t arr_hash[LEN_INSTR_SET] = {0};
    for (size_t i = 0; i < LEN_INSTR_SET; ++i)
        arr_hash[i] = asm_instr_set[i].hash;

    *index = BinSearch(arr_hash, LEN_INSTR_SET, hash_func);
    if (*index == -1)
        return UNKNOWN_CMD;
    
    return SUCCESS;
}