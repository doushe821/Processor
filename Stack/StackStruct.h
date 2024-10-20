#ifndef STACK_STRUCT_INCLUDED
#define STACK_STRUCT_INCLUDED

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "hash.h"

const int STACK_NAME_MAX = 16;
const int FUNC_NAME_MAX= 32;

struct Stack_t
{
    #ifndef NDEBUG
    int StructCanaryGuardBot;

    hash_t DataHash;
    hash_t StructHash;

    char stkName[STACK_NAME_MAX];
    char file[FILENAME_MAX];
    char func[FUNC_NAME_MAX];
    int line;

    #endif // NDEBUG

    void* data;
    size_t size;
    size_t capacity;
    size_t elSize;

    int initialised;

    u_int16_t Error;

    #ifndef NDEBUG
    int StructCanaryGuardTop;
    #endif // NDEBUG
};

#endif