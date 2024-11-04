#ifndef SPU_H_INLUDED
#define SPU_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>

#include "../Stack/Stack.h"
#include "../FileManager.h"
#include "../doublecmp.h"
#include "../Commands.h"

#include "../BigMoney.h"

struct Buffer
{
    char*  buf;
    int    err;
    size_t size;
};

struct SPU
{
    Buffer   cmdSheet;
    size_t   ip;

    Stack_t* stk;
    Stack_t* CallStk;

    double   Reg[10];
    char     ram[65536];
    
};


const uint32_t RAM_MASK = 4;
const uint32_t REG_MASK = 2;
const uint32_t C_MASK = 1;


const size_t CALLER_STACK_CAPACITY = 128;
const size_t INITIAL_STACK_CAPACITY = 512;

enum ErrCodes
{
    FOPEN_ERROR = 1,
    ASSEMBLER_ERROR,
    READING_ERROR,
    BUFFERISATION_ERROR,
    COMPILER_ERROR,
    STACK_INIT_ERROR,
    PUSH_ERROR,
    POP_ERROR,
    HEADERINIT_ERROR,
};

struct Files
{
    FILE* obj;
    bool  err;
};

struct Files CmdOpenFile(int c, char** v);
struct Buffer Bufferise(FILE* obj, const Header head);
int Process(Buffer Sheet);
int doCommand(SPU* spu);
int HeaderInit(Header* head, FILE* obj);




#endif