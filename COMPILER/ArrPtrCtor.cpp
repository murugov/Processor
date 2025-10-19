#include "compile.h"
#include "TXTreader.h"

// #define DEBUG
#include "DEBUG.h"


char** ArrPtrCtor(FILE *SourceFile, char* buffer, size_t *count_line)
{
    char **arr_ptr = TXTreader(SourceFile, buffer, count_line, toupper); // struct text
    
    ON_DEBUG(
            if (IS_BAD_PTR(arr_ptr))
                return NULL;
            )

    RemoveComments(arr_ptr, count_line);

    return arr_ptr;
}


void RemoveComments(char** arr_cmd, size_t *count_line)
{
    for (size_t i = 0; i < *count_line; ++i)
    {
        char* colon = strchr(arr_cmd[i], ';');

        if (colon != NULL)
            *colon = '\0';
    }
}


void AsmDtor(char *buffer, char **arr_ptr)
{
    free(buffer);
    free(arr_ptr);
}