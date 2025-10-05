#include "stack.h"
#include "hash.h"

StackErr_t StackInit(stk_t *stk, const char *name, const char *file, const char *func, size_t line)
{
    if (IS_BAD_PTR(stk))
    {
        StackDump(stk, __FILE__, __FUNCTION__, __LINE__);

        return ERROR;
    }
    
    (*stk).id.name = name;
    (*stk).id.file = file;
    (*stk).id.func = func;
    (*stk).id.line = line;

    return SUCCESS;
}

StackErr_t StackCtor(stk_t *stk, cap_t capacity)
{   
    stk->size = 2;
    stk->capacity = capacity + 2;

    if (ERR_DETECT(stk, STK_CTOR))
        return ERROR;
    
    stk->data = (stk_elem_t *)calloc((size_t)capacity + 2, sizeof(stk_elem_t));

    if (ERR_DETECT(stk, STK_CTOR))
        return ERROR;

    stk->data[0] = CANARY_3;
    stk->data[stk->capacity - 1] = CANARY_4;

    if(HashArrCtor())
    {
        stk->error = BAD_HASH_ARR_PTR;
        StackDump(stk, __FILE__, __FUNCTION__, __LINE__);
        
        return ERROR;
    }

    if (HashAdd(stk))
    {
        stk->error = WRONG_HASH;
        StackDump(stk, __FILE__, __FUNCTION__, __LINE__);
        
        return ERROR;
    }

    return SUCCESS;
}

StackErr_t StackPush(stk_t *stk, stk_elem_t value)
{
    if (ERR_DETECT(stk, STK_PUSH))
        return ERROR;

    if (StackRealloc(stk))
        return ERROR;

    if (ERR_DETECT(stk, STK_PUSH))
        return ERROR;

    stk->data[stk->size - 1] = value;
    stk->size++;

    hash_arr[stk->hash_index] = HashFunc(stk);

    return SUCCESS;
}

StackErr_t StackPop(stk_t *stk, stk_elem_t *last_value)
{
    stk->size--;
    
    if (ERR_DETECT(stk, STK_POP))
        return ERROR;

    *last_value = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = '\0';

    hash_arr[stk->hash_index] = HashFunc(stk);

    return SUCCESS;
}

StackErr_t StackRealloc(stk_t *stk)
{
    if (ERR_DETECT(stk, STK_CTOR))
        return ERROR;

    if (stk->size + 1 >= stk->capacity)
    {
        stk->capacity = stk->capacity << 1;
        stk_elem_t *new_data = (stk_elem_t*)realloc(stk->data, (size_t)stk->capacity * sizeof(stk_elem_t));

        if(IS_BAD_PTR(new_data))
        {
            stk->error = WRONG_REALLOC;
            StackDump(stk, __FILE__, __FUNCTION__, __LINE__);

            return ERROR;
        }

        for (size_t i = (size_t)(stk->capacity / 2) - 1; i < (size_t)stk->capacity - 1; ++i)
            new_data[i] = '\0';
        
        new_data[(size_t)stk->capacity - 1] = CANARY_4;

        stk->data = new_data;

        hash_arr[stk->hash_index] = HashFunc(stk);
    }

    return SUCCESS;
}

StackErr_t StackDtor(stk_t *stk)
{
    if (ERR_DETECT(stk, STK_DTOR))
        return ERROR;

    free(stk->data);
    free(&(hash_arr[stk->hash_index]));

    return SUCCESS;
}