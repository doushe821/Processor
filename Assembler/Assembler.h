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
#define ON_DEBUG(code) 

#endif


const uint32_t H_VERSION = 1;
const uint32_t H_SIGN = 44001;



const size_t ARGLEN_MAX = 64;
const size_t COMMANDNAME_MAX = 32; 
const size_t NUMBER_OF_COMMANDS = 32;

// FIXME fixed?

/*struct Command_t
{
    char Name[COMMANDNAME_MAX];
    char OpCode;
};*/

/*Command_t Commands[NUMBER_OF_COMMANDS]
{
    {"NULL", 0},
    {"push", 1},
    {"NULL", 2},
    {"add", 3},
    {"sub", 4},
    {"div", 5},
    {"mul", 6},
    {"pow", 7},
    {"sqrt", 8},
    {"sin", 9},
    {"NULL", 10},
    {"pop", 11},
    {"NULL", 12},
    {"NULL", 13},
    {"NULL", 14},
    {"NULL", 15},
    {"out", 16},
    {"in", 17},
    {"NULL", 18},
    {"NULL", 19},
    {"jmp", 20},
    {"jmb", 21},
    {"jma", 22},
    {"jme", 23},
    {"jmn", 24},
    {"jmbe", 25},
    {"jmae", 26},
    {"call", 27},
    {"ret", 28},
    {"sleep", 29},
    {"draw", 30},
    {"hlt", 31},
};*/ // use it

/*
!!
reuse stack for dynamic memory
!!
*/

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
    MEMORY_REALOCATION_FAILURE,
};

struct Files
{
    FILE* source;
    FILE* object;
    bool  err;
};

struct cmdSheet
{
    char*  buf;
    size_t size;
};

struct Assembly
{
    cmdSheet    sheet;
    Files       files;
    LabelTable  LTable;
    Stack_t*    LStack;
    char*       Source;
    size_t      SourceSize;
};


int Bufferize(Assembly* Asm); 


int assembly(Assembly* Asm);
struct Files CmdOpenFile(const int argc, char** argv);
int RecogniseCommand(char* buffer);
int WriteBin(FILE* obj, char* cmd, uint32_t size);

uint32_t FindReg(char* buf);

int GetCommand(Assembly* Asm, char* buffer, size_t* bread);

int ParseLabel(Assembly* Asm, char* buffer, size_t bread, char* lmarker);

int ParseJump (Assembly* Asm, char* buffer, int* cmdIndex);
int ParsePush (Assembly* Asm, char* buffer, int* cmdIndex);
int ParsePop  (Assembly* Asm, char* buffer, int* cmdindex);
int ParseCall (Assembly* Asm, char* buffer, int* cmdindex);
int ParseSleep(Assembly* Asm, char* buffer, int* cmdIndex);
int ParseDraw (Assembly* Asm, char* buffer, int* cmdIndex); // const



#endif
