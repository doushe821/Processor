#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#include "hash.h"
#include "cpybytes.h"

#ifndef NDEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code)
#endif //NDEBUG

enum errc
{
    UNDEFINED_ERROR = 2,
    ALLOC_ERROR = 4,
    DESTRUCTOR_ERROR = 8,
    FILE_CREATION_ERROR = 16,
    MEMSET_FAILURE = 32,
    STACK_UNDERFLOW = 64,
    STACK_OVERFLOW = 128,
    DATA_PTR_IS_NULL = 256,
    STACK_PTR_IS_NULL = 512,

    #ifndef NDEBUG

    BOT_DATA_CANARY_CORRUPTED = 1024, // TODO Hex
    TOP_DATA_CANARY_CORRUPTED = 2048, // TODO Hex
    BOT_STACK_CANARY_CORRUPTED = 4096, // TODO Hex
    TOP_STACK_CANARY_CORRUPTED = 8192, // TODO Hex
    DATA_CORRUPTED = 16384, // TODO Hex
    STACK_STRUCT_CORRUPTED = 32768,
    DOUBLE_INITIALISATION = 65536,

    #endif // NDEBUG
    NO_ERRORS = 0,
};

struct Stack_t;

const size_t MinCapacity = 512;
const size_t REALLOC_COEF = 2; // USED WHILE REALLOCATING MEMORY, CHANGE IF NEEDED.

int StackPush(Stack_t* stk, void* elem);
int StackPop(Stack_t* stk, void* elem);
int FStackPop(Stack_t* stk);

int StackInit(Stack_t** stk, size_t InitCapacity, size_t ElemSize);
int StackDtor(Stack_t* stk);

int StackResize(Stack_t* stk, bool downSizeFlag);
void* wrecalloc(void* ptr, size_t num, size_t size, size_t PrevSize);

int VStackPush(Stack_t* stk, const void* elem, size_t size);
int VStackInit(Stack_t** stk, size_t InitCapacity);
int StackLook(Stack_t* stk, size_t size);
size_t GetStackSize(Stack_t* stk);
void* GetDataPointer(Stack_t* stk);

int ErrParse(Stack_t* stk);

#endif