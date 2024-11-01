#ifndef COMMAND_HANDLER_H_INCLUDED
#define COMMAND_HANDLER_H_INCLUDEED

#include <stdlib.h>
#include "../Stack//Stack.h"

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

int HandlePush  (SPU* Baikal);
int HandleAdd   (SPU* Baikal);
int HandleSub   (SPU* Baikal);
int HandleDiv   (SPU* Baikal);
int HandleMul   (SPU* Baikal);
int HandlePow   (SPU* Baikal);
int HandleSqrt  (SPU* Baikal);
int HandleSin   (SPU* Baikal);
int HandleOut   (SPU* Baikal);
int HandlePop   (SPU* Baikal);
int HandleJmp   (SPU* Baikal);
int HandleJmb   (SPU* Baikal);
int HandleJma   (SPU* Baikal);
int HandleJme   (SPU* Baikal);
int HandleJmn   (SPU* Baikal);
int HandleJmbe  (SPU* Baikal);
int HandleJmae  (SPU* Baikal);
int HandleCall  (SPU* Baikal);
int HandleRet   (SPU* Baikal);
int HandleSleep (SPU* Baikal);
int HandleDraw  (SPU* Baikal);
int HandleIn    (SPU* Baikal);
int HandleHlt   (SPU* Baikal);

int HandleUnknown(SPU* Baikal);

#endif