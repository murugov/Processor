#include "compile.h"

// #define DEBUG
#include "DEBUG.h"


// AsmErr_t CmdWithArg(asm_context *write_params)
AsmErr_t CmdWithArg(unsigned char *code, char *ptr, size_t *pc, CmdCodes cmd, label_t *arr_labels, size_t *count_labels, bool is_first_pass)
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

    union {
        arg_t value;
        unsigned char bytes[sizeof(arg_t)];
    } converter;

    if (sscanf(ptr, "%*s %d ", &arg_num))
    {
        if (is_first_pass)
        {
            code[*pc] = (cmd) | OP_NUM;
            converter.value = arg_num;
            
            for (size_t byte = 0; byte < sizeof(arg_t); ++byte)
            {
                (*pc)++;
                code[*pc] = converter.bytes[byte];
            }
        }
        else
            (*pc) += sizeof(arg_t);
    }

    else if (sscanf(ptr, "%*s R%d ", &arg_reg))
    {
        if ((is_first_pass) && 0 <= arg_reg && arg_reg < NUM_REG)
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
        if (sscanf(label, "%d", &arg_num))
        {
            if (is_first_pass)
            {
                code[*pc] = (cmd) | OP_MEM | OP_NUM;
                converter.value = arg_num;
                
                for (size_t byte = 0; byte < sizeof(arg_t); ++byte)
                {
                    (*pc)++;
                    code[*pc] = converter.bytes[byte];
                }
            }
            else
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
            converter.value = -1;
            
            for (size_t byte = 0; byte < sizeof(arg_t); ++byte)
            {
                (*pc)++;
                code[*pc] = converter.bytes[byte];
            }
        }

        else
        {
            for (size_t lb = 0; lb < *count_labels; ++lb)
            {
                hash_t name_hash = HashCmd(arr_labels[lb].name);
                hash_t label_hash = HashCmd(label);
    
                if (name_hash == label_hash)
                {
                    code[*pc] = cmd | OP_NUM;
                    converter.value = (arg_t)arr_labels[lb].pc;// нужен union для *pc - size_t
                    
                    for (size_t byte = 0; byte < sizeof(arg_t); ++byte) // как бы я должен size_t держать, иначе мало смысла в моём stk_ret
                    {
                        (*pc)++;
                        code[*pc] = converter.bytes[byte];
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


// AsmErr_t CmdWithoutArg(asm_context *write_params)
AsmErr_t CmdWithoutArg(unsigned char *code, char * /*ptr*/, size_t *pc, CmdCodes cmd, label_t * /*arr_labels*/, size_t * /*count_labels*/, bool is_first_pass)
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