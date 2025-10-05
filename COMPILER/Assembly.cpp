#include "compile.h"
#include "colors.h"

AsmErr_t Assembly(char **arr_ptr, size_t count_n, FILE *ByteCode)
{
    proc_elem_t *cmds = (proc_elem_t*)calloc(count_n * 2, sizeof(proc_elem_t));
    if (IS_BAD_PTR(cmds))
        return BAD_ARR_CMDS_PTR;

    size_t counter = 0;

    for (size_t i = 0; i < count_n; ++i)
    {
        hash_t func = HashCmd(arr_ptr[i]);

        AsmErr_t write_verd = TaskWriter(cmds, arr_ptr[i], func, &counter);

        if (write_verd == UNKNOWN_CMD)
        {
            printf(ANSI_COLOR_RED "Unknown command in source.asm:%zu\n" ANSI_COLOR_RESET, i + 1); // return file
            free(cmds);
            return ERROR;
        }
    }

    const char* signature = SIGNATURE;
    short version = VERSION;

    fwrite(signature, 2, sizeof(char), ByteCode);
    fwrite(&version, 1, sizeof(short), ByteCode);
    fwrite(cmds, counter, sizeof(proc_elem_t), ByteCode);

    free(cmds);
        
    return SUCCESS;
}

AsmErr_t TaskWriter(proc_elem_t *cmds, char *ptr, hash_t func, size_t *counter)
{
    switch (func)
    {
        case cmd_PUSH:
            cmds[*counter] = Inv_cmd_PUSH;
            (*counter)++;
            sscanf(ptr, "%*s %d ", &cmds[*counter]);
            break;

        case cmd_ADD:
            cmds[*counter] = Inv_cmd_ADD;
            break;

        case cmd_SUB:
            cmds[*counter] = Inv_cmd_SUB;
            break;

        case cmd_MUL:
            cmds[*counter] = Inv_cmd_MUL;
            break;

        case cmd_DIV:
            cmds[*counter] = Inv_cmd_DIV;
            break;

        case cmd_POW:
            cmds[*counter] = Inv_cmd_POW;
            break;

        case cmd_MOD:
            cmds[*counter] = Inv_cmd_MOD;
            break;

        case cmd_SQRT:
            cmds[*counter] = Inv_cmd_SQRT;
            break;
            
        case cmd_OUT:
            cmds[*counter] = Inv_cmd_OUT;
            break;

        case cmd_HLT:
            cmds[*counter] = Inv_cmd_HLT;
            break; 

        default:
            return UNKNOWN_CMD;
    }
    
    (*counter)++;

    return SUCCESS;
}