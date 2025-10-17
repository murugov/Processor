#include "compile.h"
#include "TXTreader.h"

// #define DEBUG
#include "DEBUG.h"


void rmcom(char** arr_cmd, size_t *count_line)
{
    for (size_t i = 0; i < *count_line; ++i)
    {
        char* com = strchr(arr_cmd[i], ';');

        if (com != NULL)
            *com = '\0';
    }
}

char** ArrPtrCtor(FILE *SourceFile, size_t *count_line)
{
    char **arr_ptr = TXTreader(SourceFile, count_line, toupper);
    
    ON_DEBUG(
            if (IS_BAD_PTR(arr_ptr))
                return NULL;
            )
    // inline спросить у деда

    rmcom(arr_ptr, count_line);

    return arr_ptr;
}