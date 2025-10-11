#include "compile.h"


AsmErr_t CmdWithArg(char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass)
{
    if (IS_BAD_PTR(code))
        return CMD_WITH_ARG_FAIL;

    if (IS_BAD_PTR(ptr))
        return CMD_WITH_ARG_FAIL;

    if (IS_BAD_PTR(pc))
        return CMD_WITH_ARG_FAIL;

    if (IS_BAD_PTR(arr_labels))
        return CMD_WITH_ARG_FAIL;

    if (IS_BAD_PTR(count_labels))
        return CMD_WITH_ARG_FAIL;

    cmd_arg_t arg_c = 0; //arg_num
    cmd_arg_t arg_r = 0; //arg_reg
    char label[32] = "\0";

    if (sscanf(ptr, "%*s %d ", &arg_c))
    {
        if (is_first_pass)
        {
            code[*pc] = cmd | OP_NUM;
            for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
            {
                (*pc)++;
                code[*pc] = (char)((arg_c >> (8 * byte)) & 0xFF);
            }
        }
        else
            (*pc) += sizeof(cmd_arg_t);
    }

    else if (sscanf(ptr, "%*s R%d ", &arg_r))
    {
        if (is_first_pass && 0 <= arg_r && arg_r < NUM_REG)
        {
            code[*pc] = cmd | OP_REG;
            (*pc)++;
    
            code[*pc] = (char)arg_r;
        }
        else if (!is_first_pass)
            (*pc)++;
        else
            return REG_NEX;
    }

    else if (sscanf(ptr, "%*s %s: ", label))
    {
        if (is_first_pass)
        {
            code[*pc] = cmd | OP_NUM;
            for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
            {
                (*pc)++;
                code[*pc] = (char)(((-1) >> (8 * byte)) & 0xFF);
            }
        }

        else
        {
            for (size_t lb = 0; lb < *count_labels; ++lb)
            {
                // printf("name = %s\n", arr_labels[lb].name);
                // printf("label = %s\n", label);
                hash_t name_hash = HashCmd(arr_labels[lb].name);
                hash_t label_hash = HashCmd(label) * 31 + (size_t)':'; //????????????????????????????
    
                if (name_hash == label_hash)
                {
                    code[*pc] = cmd | OP_NUM;
                    for (size_t byte = 0; byte < sizeof(cmd_arg_t); ++byte)
                    {
                        (*pc)++;
                        code[*pc] = (char)((arr_labels[lb].pc >> (8 * byte)) & 0xFF);
                    }
                    
                    (*pc)++;

                    return SUCCESS;
                }
            }

            return UNKNOWN_LABEL;
        }
    }

    else
        return UNKNOWN_CMD;
    
    (*pc)++;

    return SUCCESS;

}


AsmErr_t CmdWithoutArg(char *code, char * /*ptr*/, size_t *pc, CmdCodes cmd, label_t * /*arr_labels*/, size_t * /*count_labels*/, bool is_first_pass)
{
    if (IS_BAD_PTR(code))
        return CMD_WITHOUT_ARG_FAIL;

    if (IS_BAD_PTR(pc))
        return CMD_WITHOUT_ARG_FAIL;

    if (is_first_pass)
        code[*pc] = cmd;

    (*pc)++;

    return SUCCESS;
}