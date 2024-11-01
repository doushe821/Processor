#ifndef COMMAND_PARSER_H_INCLUDED
#define COMMAND_PARSER_H_INCLUDED

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


int ParseJump (Assembly_t* Asm, char* buffer);
int ParsePush (Assembly_t* Asm, char* buffer);
int ParsePop  (Assembly_t* Asm, char* buffer);
int ParseCall (Assembly_t* Asm, char* buffer);
int ParseSleep(Assembly_t* Asm, char* buffer);
int ParseDraw (Assembly_t* Asm, char* buffer); 
int ParseNoArg(Assembly_t* Asm, char* buffer);

int ParseUnknown(Assembly_t* Asm, char* buffer);


#endif