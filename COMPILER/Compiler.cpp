#include "compile.h"
#include "SizeFile.h"


AsmErr_t Compiler(FILE *SourceFile, FILE *ByteCode)
{
    if (IS_BAD_PTR(SourceFile))
        return BAD_INPUT_FILE_PTR;

    if (IS_BAD_PTR(ByteCode))
        return BAD_OUTPUT_FILE_PTR;

    ssize_t file_size = SizeFile(SourceFile);
    
    if (file_size < 0)
        return WRONG_FILE_SIZE;

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IS_BAD_PTR(buffer))
        return BAD_BUFFER_PTR;

    size_t capacity = fread(buffer, sizeof(char), (size_t)file_size, SourceFile);
    buffer[capacity] = '\0';

    for (size_t num_elem = 0; num_elem < (size_t)file_size; ++num_elem) //new func
        buffer[num_elem] = (char)toupper(buffer[num_elem]);

    size_t count_cmd = CmdNumber(buffer); //new name LineNumber

    if (count_cmd == 0)
        return CMD_NUM_FAIL;

    char **arr_cmd = (char**)calloc(count_cmd + 1, sizeof(char*));
    if (IS_BAD_PTR(arr_cmd))
        return BAD_ARR_CMD_PTR;

    if (ArrPtrCtor(buffer, arr_cmd)) // new file
        return CTOR_FAIL;
    
    AsmErr_t asm_verd = Assembler(arr_cmd, count_cmd, ByteCode); // в main

    free(buffer);
    free(arr_cmd);

    if (asm_verd)
        return asm_verd;

    return SUCCESS;
}


AsmErr_t ArrPtrCtor(char *buffer, char **arr_cmd)
{
    if (IS_BAD_PTR(buffer))
        return CTOR_FAIL;
    
    size_t count = 0;
    char* line_start = buffer;
    char* next_n = buffer;
    
    while ((next_n = strchr(line_start, '\n')) != NULL)
    {
        int has_content = 0;

        for (char* ptr = line_start; ptr < next_n; ++ptr)
        {
            if (!isspace((unsigned char)*ptr))
            {
                has_content = 1;
                arr_cmd[count] = ptr;
                break;
            }
        }
        
        if (has_content)
            count++;
        
        line_start = next_n + 1;

        *next_n = '\0';
    }
    
    if (*line_start != '\0')
    {
        for (char* ptr = line_start; *ptr != '\0'; ++ptr)
        {
            if (!isspace((unsigned char)*ptr)) 
            {
                arr_cmd[count++] = ptr;
                break;
            }
        }
    }

    for (size_t i = 0; i < count; ++i)
    {
        char* com = strchr(arr_cmd[i], ';');

        if (com != NULL)
            *com = '\0';
    }

    return SUCCESS;
}


size_t CmdNumber(char* buffer)
{
    if (IS_BAD_PTR(buffer))
        return 0;
    
    size_t count_cmd = 0;
    char* line_start = buffer;
    char* next_n = buffer;
    
    while ((next_n = strchr(line_start, '\n')) != NULL)
    {
        int has_content = 0;

        for (char* ptr = line_start; ptr < next_n; ++ptr)
        {
            if (!isspace((unsigned char)*ptr))
            {
                has_content = 1;
                break;
            }
        }
        
        if (has_content)
            count_cmd++;
        
        line_start = next_n + 1;
    }
    
    if (*line_start != '\0')
    {
        for (char* ptr = line_start; *ptr != '\0'; ++ptr)
        {
            if (!isspace((unsigned char)*ptr)) 
            {
                count_cmd++;
                break;
            }
        }
    }
    
    return count_cmd;
}