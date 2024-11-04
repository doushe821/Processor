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

#define NDEBUG

#include "../Stack/Stack.h"
#include "../Stack/StackDB.h"
#include "../Commands.h"


#ifndef NDEBUG

#include "../BigMoney.h"

#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code) 

#endif


const uint32_t H_VERSION = 1;
const uint32_t H_SIGN = 44001;
const int PARSE_FUNC_NUMBER = 7;


struct Files
{
    FILE* source;
    FILE* object;
    bool  err;
};

struct Assembly_t
{
    Stack_t*    cmdStack;
    Files       files;
    LabelTable  LTable;
    Stack_t*    LStack;
    char*       Source;
    size_t      SourceSize;
};


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
    HANDLER_ERROR,
    CORRUPTED_LABELS,
};



int Bufferize(Assembly_t* Asm); 
int Assembly (Assembly_t* Asm);


struct Files CmdOpenFile(const int argc, char** argv);

int WriteBin(FILE* obj, Stack_t* cmdStack);
uint32_t FindReg(char* buf);

int RecogniseCommand(char* buffer);

int GetCommand(Assembly_t* Asm, char* buffer, size_t* bread);

int ParseCommand(Assembly_t* Asm, int code, char* buffer, size_t* bread);

int ParseLabel(Assembly_t* Asm, char* buffer, size_t bread, char* lmarker);

#endif
