#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IsBadPtr.h"

enum StackErr_t
{
    STK_SUCCESS = 0,
    STK_ERROR = 1
};

enum ErrCodes_t
{
    WRONG_CANARY = 1,
    WRONG_HASH = 2,
    BAD_HASH_ARR_PTR = 4,
    BAD_STK_PTR = 8,
    BAD_DATA_PTR = 16,
    WRONG_SIZE = 32,
    WRONG_CAPACITY = 64,
    DATA_OVERFLOW = 128,
    ACCESS_EMPTY_DATA = 256,
    BAD_INCREASED_STK_PTR = 512,
    WRONG_REALLOC = 1024
};

enum StackFunc
{
    DEFUALT = 0,
    STK_CTOR = 1,
    STK_DTOR = 2,
    STK_POP = 3,
    STK_PUSH = 4,
    STK_REALLOC = 5
};

enum Canary
{
    CANARY_1 = 111,
    CANARY_2 = 222,
    CANARY_3 = 333,
    CANARY_4 = 444
};

typedef int stk_elem_t;
typedef ssize_t cap_t;
typedef ssize_t err_t;
typedef stk_elem_t canary_t;

struct stack_id
{
    const char *name;
    const char *file;
    const char *func;
    size_t line;
};

struct stk_t
{
    canary_t canary_1 = CANARY_1;
    stk_elem_t *data;
    ssize_t size;
    cap_t capacity;
    err_t error;
    struct stack_id id;
    canary_t canary_2 = CANARY_2;
    size_t hash_index;
};

StackErr_t StackInit(stk_t *stk, const char *name, const char *file, const char *func, size_t line);
StackErr_t ErrDetect(stk_t *stk, StackFunc IncomingFunc, const char *file, const char *func, size_t line);
StackErr_t StackCtor(stk_t *stk, cap_t capacity);
StackErr_t StackDtor(stk_t *stk);
void StackDump(stk_t *stk, const char *file, const char *func, size_t line);
StackErr_t StackPop(stk_t *stk, stk_elem_t *last_value);
StackErr_t StackPush(stk_t *stk, stk_elem_t value);
StackErr_t StackRealloc(stk_t *stk);
StackErr_t StackVerify(stk_t *stk, StackFunc IncomingFunc = DEFUALT);

#define ERR_DETECT(stk, IncomingFunc)  ErrDetect(stk, IncomingFunc, __FILE__, __FUNCTION__, __LINE__)
#define ERR_CHECK(err_code)  ((stk->error) & err_code) == err_code
#define STACK_INIT(stk, capacity)  StackInit(stk, #stk, __FILE__, __FUNCTION__, __LINE__); StackCtor(stk, capacity)
#define IS_BAD_PTR(ptr) IsBadPtr((void*)ptr)

#endif