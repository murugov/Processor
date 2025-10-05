#ifndef HASH_H
#define HASH_H

#include "stack.h"

typedef size_t hash_t;

extern hash_t *hash_arr;

hash_t HashFunc(stk_t *stk);
hash_t HashCmd(char *buffer);
StackErr_t HashArrCtor();
StackErr_t HashAdd(stk_t *stk);
StackErr_t HashArrRealloc();
//void HashArrDtor(); // Special function for array of stacks

#endif