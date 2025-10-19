#include "compile.h"

// #define DEBUG
#include "DEBUG.h"


// AsmErr_t CmdWithArg(asm_context *write_params)
AsmErr_t CmdWithArg(byte_t *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass)
{
    ON_DEBUG(
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
            )

    arg_t arg_num = 0;
    arg_t arg_reg = 0;
    char label[32] = "\0";

    if (sscanf(ptr, "%*s %d ", &arg_num))
    {
        if (is_first_pass)
        {
            code[*pc] = (cmd) | OP_NUM;            
            memcpy(&code[*pc + 1], &arg_num, sizeof(arg_t));
        }
        
        (*pc) += sizeof(arg_t);
    }

    else if (sscanf(ptr, "%*s R%d ", &arg_reg))
    {
        if ((is_first_pass) && 0 <= arg_reg && arg_reg < NUM_REG) // reverse condition
        {
            code[*pc] = (cmd) | OP_REG;
            (*pc)++;
    
            code[*pc] = (unsigned char)arg_reg;
        }
        else if (!(is_first_pass))
            (*pc)++;
        else
            return REG_NEX;
    }

    else if (sscanf(ptr, "%*s [%s] ", label))
    {
        if (sscanf(label, " %d ", &arg_num))
        {
            if (is_first_pass)
            {
                code[*pc] = (cmd) | OP_NUM;            
                memcpy(&code[*pc + 1], &arg_num, sizeof(arg_t));
            }
            
            (*pc) += sizeof(arg_t);
        }

        else if (sscanf(label, "R%d", &arg_reg))
        {
            if ((is_first_pass) && 0 <= arg_reg && arg_reg < NUM_REG)
            {
                code[*pc] = (cmd) | OP_MEM | OP_REG;
                (*pc)++;
        
                code[*pc] = (unsigned char)arg_reg;
            }
            else if (!is_first_pass)
                (*pc)++;
            else
                return REG_NEX;
        }

        else
            return MEM_NEX;
    }

    else if (sscanf(ptr, "%*s %s ", label) && (CMD_JMP <= (cmd) && (cmd) <= CMD_CALL))
    {
        if (is_first_pass)
        {
            code[*pc] = (cmd) | OP_NUM;
            arg_num = -1;
            memcpy(&code[*pc + 1], &arg_num, sizeof(arg_t));

            (*pc) += sizeof(arg_t);
        }

        else
        {
            for (size_t lb = 0; lb < *count_labels; ++lb)
            {
                hash_t label_hash = HashCmd(label) * 31 + ':';       // very bad, may be define
    
                if (arr_labels[lb].hash == label_hash)
                {
                    code[*pc] = (cmd) | OP_NUM;
                    arg_num = (arg_t)arr_labels[lb].pc;
                    memcpy(&code[*pc + 1], &arg_num, sizeof(arg_t));

                    (*pc) += 1 + sizeof(arg_t);

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


// AsmErr_t CmdWithoutArg(asm_context *write_params)
AsmErr_t CmdWithoutArg(byte_t *code, char * /*ptr*/, size_t *pc, CmdCodes cmd, label_t * /*arr_labels*/, size_t * /*count_labels*/, bool is_first_pass)
{
    ON_DEBUG(
            if (IS_BAD_PTR(code))
                return CMD_WITHOUT_ARG_FAIL;

            if (IS_BAD_PTR(pc))
                return CMD_WITHOUT_ARG_FAIL;
            )

    if (is_first_pass)
        code[*pc] = cmd;

    (*pc)++;

    return SUCCESS;
}