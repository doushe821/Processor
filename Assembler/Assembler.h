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

int FindReg(char* buf);

const char CommandNames[32][COMMANDNAME_MAX]    
{"", "push", "", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "", "pop", "", "", "", "", "out", "ret", 
 "", "", "jmp", "jmb", "jma","jme", "jmn", "jmbe", "jmae", "", "",
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
    int* buf;
    size_t size;
};

struct Assembly
{
    cmdSheet sheet;
    Files files;
    LabelTable LTable;
    Stack_t* LStack;
};



int assembly(Assembly* Asm);
struct Files CmdOpenFile(int c, char** v);
int RecogniseCommand(char* buffer);
int WriteBin(FILE* obj, int* cmd, uint32_t size);
int ParseLabel(Assembly* Asm, char* lmarker, char* buffer, int cycleIndex);
int ParseJump(Assembly* Asm, int** cmdSheet, char* buffer, int* cycleIndex);
int ParsePush(Assembly* Asm, char* buffer, int* cycleIndex, int** cmdSheet);
int ParsePop(Assembly* Asm, char* buffer, int* cycleIndex, int** cmdSheet);
int ParseRet(Assembly* Asm, int* cycleIndex, int** cmdSHeet);
int ParseSleep(Assembly* Asm, int* cycleIndex, int** cmdSheet);
int ParseDraw(Assembly* Asm, int* cycleIndex, int** cmdSheet);




#endif
