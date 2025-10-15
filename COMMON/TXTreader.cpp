#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IsBadPtr.h"
#include "SizeFile.h"
#include "LineCounter.h"


int no_change(int c)
{
    return c;
}

char** TXTreader(FILE *SourceFile, size_t *count_line, int (*convert)(int))
{
    if (IsBadPtr((void*)SourceFile))
        return NULL;

    ssize_t file_size = SizeFile(SourceFile);
    if (file_size < 0)
        return NULL;

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IsBadPtr((void*)buffer))
        return NULL;

    size_t capacity = fread(buffer, sizeof(char), (size_t)file_size, SourceFile);
    buffer[capacity] = '\0';

    if (convert == NULL)
        convert = no_change;

    for (size_t num_elem = 0; num_elem < capacity; ++num_elem)
        buffer[num_elem] = (char)convert(buffer[num_elem]);

    *count_line = LineCounter(buffer);
    if (*count_line == 0)
        return NULL;

    char **arr_ptr = (char**)calloc(*count_line + 1, sizeof(char*));
    if (IsBadPtr((void*)arr_ptr))
        return NULL;
    
    size_t line_number = 0;
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
                arr_ptr[line_number] = ptr;
                break;
            }
        }
        
        if (has_content)
            line_number++;
        
        line_start = next_n + 1;

        *next_n = '\0';
    }
    
    if (*line_start != '\0')
    {
        for (char* ptr = line_start; *ptr != '\0'; ++ptr)
        {
            if (!isspace((unsigned char)*ptr)) 
            {
                arr_ptr[line_number++] = ptr;
                break;
            }
        }
    }

    // free(buffer); //AAAAASSSSSSKKKKKK

    return arr_ptr;
}