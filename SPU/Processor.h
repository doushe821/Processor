#ifndef SPU_INLUDED
#define SPU_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <inttypes.h>

#include "../Stack/Stack.h"
#include "../FileManager.h"
#include "../Commands.h"

#include "../BigMoney.h"




const uint32_t RAM_MASK = 4;
const uint32_t REG_MASK = 2;
const uint32_t C_MASK = 1;




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



struct Buffer
{
    int* buf;
    int err;
    size_t size;
};

struct SPU
{
    Buffer cmdSheet;
    Stack_t* stk;
    int Reg[10];
    size_t ip;
    int ram[131072];
};


struct Files
{
    FILE* obj;
    bool err;
};

struct Files CmdOpenFile(int c, char** v);
struct Buffer Bufferise(FILE* obj, const Header head);
int Process(Buffer Sheet);
int doCommand(SPU* spu);
int HeaderInit(Header* head, FILE* obj);


#endif