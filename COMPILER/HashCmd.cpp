#include "compile.h"


hash_t HashCmd(const char *cmd)
{
    if (IS_BAD_PTR(cmd))
        return 0;
    
    hash_t new_hash = 0;
    
    while (*cmd != ' ' && *cmd != '\0')
    {
        new_hash = (new_hash << 5) - new_hash + (hash_t)(*cmd);
        cmd++;
    }

    return new_hash;
}