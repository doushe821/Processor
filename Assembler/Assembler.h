#ifndef ASSEMBLER_INCLUDED
#define ASSEMBLER_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#include "../FileManager.h"
#include "LabelTable.h"
#include "../Commands.h"
#include "../Stack/Stack.h"
#include "../Stack/StackDB.h"

#ifndef NDEBUG


#include "../BigMoney.h"


#define ON_DEBUG(code) code
#else
#define ON_DEBUG() 
#endif


const uint32_t version = 1;
const uint32_t sign = 44001;



const size_t ARGLEN_MAX = 64;
const size_t COMMANDNAME_MAX = 32; 


const char CommandNames[32][COMMANDNAME_MAX]    
{"", "push", "", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "", "pop", "", "", "", "", "out", "ret", 
 "", "", "jmp", "jmb", "jma","jme", "jmn", "jmbe", "jmae", "call", "ret",
 "sleep", "draw", "hlt"};

enum ErrCodes
{
    FOPEN_ERROR = 1,
    ASSEMBLER_ERROR,
    READING_ERROR,
    LTABLE_CREATION_ERROR,
    CALLOC_ERROR,
    BINARY_WRITING_ERROR,
    UNDEFINED_LABEL,
    STACK_CREATION_ERROR,
    SYNTAX_ERROR,
};

struct Files // RENAME LATER
{
    FILE* source;
    FILE* object;
    bool err;
};

struct cmdSheet
{
    char* buf;
    size_t size;
};

struct Assembly
{
    cmdSheet sheet;
    Files files;
    LabelTable LTable;
    Stack_t* LStack;
    char* Source;
    size_t SourceSize;
};

struct Token 
{
    char command[COMMANDNAME_MAX];
    size_t size;
    char* endptr;
    char* startptr;
    char type;
    size_t objSize;
    size_t ip;
};


int Bufferize(Assembly* Asm); 


int assembly(Assembly* Asm);
struct Files CmdOpenFile(int c, char** v);
int RecogniseCommand(char* buffer);
int WriteBin(FILE* obj, char* cmd, uint32_t size);

uint32_t FindReg(char* buf);

int GetCommand(Assembly* Asm, char* buffer, size_t* bread);

int ParseLabel(Assembly* Asm, char* buffer, size_t bread, char* lmarker);

int ParseJump (Assembly* Asm, char* buffer, int* cmdIndex);
int ParsePush (Assembly* Asm, char* buffer, int* cmdIndex);
int ParsePop  (Assembly* Asm, char* buffer, int* cmdindex);

int ParseRet  (Assembly* Asm,               int* cycleIndex, char** cmdSHeet);
int ParseSleep(Assembly* Asm, int* cycleIndex, int** cmdSheet);
int ParseDraw (Assembly* Asm,               int* cycleIndex, char** cmdSheet);




#endif
