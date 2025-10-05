#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
        return WRONG_FILE_INFO;

    char* buffer = (char*)calloc((size_t)file_size + 2, sizeof(char));
    if (IS_BAD_PTR(buffer))
        return BAD_BUFFER_PTR;

    size_t capacity = fread(buffer, sizeof(char), (size_t)file_size, SourceFile);
    buffer[capacity] = '\0';

    size_t count_n = CmdNumber(buffer);

    if (count_n == 0)
        return CMD_NUM_FAIL;

    char **arr_ptr = (char**)calloc(count_n + 1, sizeof(char*));
    
    if (IS_BAD_PTR(arr_ptr))
        return BAD_ARR_PTR;

    if (ArrPtrCtor(buffer, arr_ptr))
        return BAD_BUFFER_PTR;
    
    AsmErr_t asm_verd = Assembly(arr_ptr, count_n, ByteCode);
    if (asm_verd)
        return asm_verd;

    free(buffer);
    free(arr_ptr);

    return SUCCESS;
}

size_t CmdNumber(char* buffer)
{
    if (IS_BAD_PTR(buffer))
        return 0;
    
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
                break;
            }
        }
        
        if (has_content)
            count++;
        
        line_start = next_n + 1;
    }
    
    if (*line_start != '\0')
    {
        for (char* ptr = line_start; *ptr != '\0'; ++ptr)
        {
            if (!isspace((unsigned char)*ptr)) 
            {
                count++;
                break;
            }
        }
    }
    
    return count;
}

AsmErr_t ArrPtrCtor(char *buffer, char **arr_ptr)
{
    if (IS_BAD_PTR(buffer))
        return ERROR;
    
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
                arr_ptr[count] = ptr;
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
                arr_ptr[count++] = ptr;
                break;
            }
        }
    }

    for (size_t i = 0; i < count; ++i)
    {
        char* com = strchr(arr_ptr[i], ';');

        if (com != NULL)
            *com = '\0';
    }

    return SUCCESS;
}