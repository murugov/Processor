#ifndef HASH_TABLE_FUNC_HPP
#define HASH_TABLE_FUNC_HPP

#include "hash_table.hpp"


template <typename htElem_T>
htErr_t htInit(ht_t<htElem_T> *hash_table, const char *name, const char *file, const char *func, int line)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "BAD_HT_PTR"); return HT_ERROR; } )
    
    hash_table->id.name = name;
    hash_table->id.file = file;
    hash_table->id.func = func;
    hash_table->id.line = line;

    ON_DEBUG( LOG(DEBUG, "Created id for hash-table \"%s\"", hash_table->id.name); )
    return HT_SUCCESS;
}


template <typename htElem_T>
htErr_t htCtor(ht_t<htElem_T> *hash_table)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "BAD_HT_PTR"); return HT_ERROR; } )
    
    hash_table->table = (table_t<htElem_T>*)calloc(HT_SIZE, sizeof(table_t<htElem_T>));
    if (IS_BAD_PTR(hash_table->table)) { 
        LOG(ERROR, "Failed to allocate table");
        return HT_ERROR; 
    }
    
    for (int i = 0; i < HT_SIZE; ++i)
    {
        hash_table->table[i].is_used = 0;
        hash_table->table[i].stk = NULL;
    }
    
    ON_DEBUG( LOG(INFO, "Hash-table \"%s\" created", hash_table->id.name); )
    return HT_SUCCESS;
}


template <typename htElem_T>
htErr_t htDtor(ht_t<htElem_T> *hash_table)
{
    ON_DEBUG( if (IS_BAD_PTR(hash_table)) { LOG(ERROR, "BAD_HT_PTR"); return HT_ERROR; } )

    if (hash_table->table == NULL)
    {
        ON_DEBUG( LOG(WARN, "Hash-table \"%s\" already destroyed or not initialized", hash_table->id.name); )
        return HT_SUCCESS;
    }

    for (int i = 0; i < HT_SIZE; ++i)
    {
        if (hash_table->table[i].stk != NULL)
        {
            STACK_DTOR(hash_table->table[i].stk);
            free(hash_table->table[i].stk);
            hash_table->table[i].stk = NULL;
        }
        hash_table->table[i].is_used = 0;
    }
    free(hash_table->table);
    hash_table->table = NULL;

    ON_DEBUG( LOG(INFO, "Hash-table \"%s\" destroyed", hash_table->id.name); )
    
    hash_table->id.name = NULL;
    hash_table->id.file = NULL;
    hash_table->id.func = NULL;
    hash_table->id.line = 0;

    return HT_SUCCESS;
}


template <typename htElem_T>
const char *htFind(ht_t<htElem_T> *hash_table, const char *target, int (*hashTableComparator)(const void*, const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(target)) { LOG(ERROR, "Bad pointer in htFind"); return NULL; } )

    hash_t hash_target = GetHash(target) & (HT_SIZE - 1);

    if (hash_table->table[hash_target].is_used == 0) { return NULL; }
    
    stk_t<htElem_T> *stk = hash_table->table[hash_target].stk;
    if (IS_BAD_PTR(stk)) { return NULL; }

    for (ssize_t i = 0; i < stk->size; ++i)
    {
        htElem_T current_elem = stk->data[i];
        if (!hashTableComparator(&current_elem, target))
        {
            return stk->data[i];
        }
    }

    return NULL;
}


template <typename htElem_T>
htErr_t htInsert(ht_t<htElem_T> *hash_table, const char *item, int (*hashTableComparator)(const void*, const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(item)) { LOG(ERROR, "Bad pointer in htInsert"); return HT_ERROR; } )

    hash_t hash_item = GetHash(item) & (HT_SIZE - 1);

    if (hash_table->table[hash_item].is_used == 0)
    {
        hash_table->table[hash_item].is_used = 1;
        stk_t<htElem_T> *new_stk = (stk_t<htElem_T>*)calloc(1, sizeof(stk_t<htElem_T>));
        if (IS_BAD_PTR(new_stk))
        {
            for (int i = 0; i < HT_SIZE; ++i)
            {
                if (hash_table->table[i].stk != NULL)
                {
                    STACK_DTOR(hash_table->table[i].stk);
                    free(hash_table->table[i].stk);
                }
            }
            free(hash_table->table);
            hash_table->table = NULL;
            return HT_ERROR;
        }
    
        STACK_CTOR(new_stk, INIT_NUM_COL);
        ON_DEBUG(
            if (new_stk->error != STK_NO_ERRORS)
            {
                LOG(ERROR, "Stack construction failed");
                free(new_stk);
                return HT_ERROR;
            }
        )
        hash_table->table[hash_item].stk = new_stk;
    }
    else
    {
        stk_t<htElem_T> *stk = hash_table->table[hash_item].stk;
        if (IS_BAD_PTR(stk)) { return HT_ERROR; }

        for (ssize_t i = 0; i < stk->size; ++i)
        {
            htElem_T current_elem = stk->data[i];
            if (!hashTableComparator(&current_elem, item)) { return HT_ERROR; }
        }
    }

    StackPush(hash_table->table[hash_item].stk, item);

    return HT_SUCCESS;
}


template <typename htElem_T>
htErr_t htRemove(ht_t<htElem_T> *hash_table, const char *item, int (*hashTableComparator)(const void*, const char*))
{
    ON_DEBUG( if(IS_BAD_PTR(hash_table) || IS_BAD_PTR(item)) { LOG(ERROR, "Bad pointer in htRemove"); return HT_ERROR; } )

    hash_t hash_item = GetHash(item) & (HT_SIZE - 1);

    if (hash_table->table[hash_item].is_used == 0 || hash_table->table[hash_item].stk == NULL)
    { 
        LOG(DEBUG, "Item not found - empty bucket");
        return HT_ERROR; 
    }

    stk_t<htElem_T> stk_ret = {};
    STACK_CTOR(&stk_ret, INIT_NUM_COL);
    if (stk_ret.error != STK_NO_ERRORS)
    { 
        LOG(ERROR, "Failed to create temp stack");
        return HT_ERROR;
    }

    bool found = false;
    htElem_T current_elem = NULL;
    stk_t<htElem_T> *stk = hash_table->table[hash_item].stk;
    
    while (stk->size > 0)
    {
        StackPop(stk, &current_elem);
        
        if (!hashTableComparator(&current_elem, item))
        {
            found = true;
            break;
        }
        else
        {
            StackPush(&stk_ret, current_elem);
        }
    }

    while (stk_ret.size > 0)
    {
        StackPop(&stk_ret, &current_elem);     
        StackPush(stk, current_elem);
    }

    STACK_DTOR(&stk_ret);

    if (stk->size == 0)
    {
        hash_table->table[hash_item].is_used = 0;
        STACK_DTOR(hash_table->table[hash_item].stk);
        free(hash_table->table[hash_item].stk);
        hash_table->table[hash_item].stk = NULL;
    }

    return found ? HT_SUCCESS : HT_ERROR;
}

#endif