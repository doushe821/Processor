#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "FileManager.h"
#include "LabelTable.h"
#include "Commands.h"
#include "Stack.h"
#include "StackDB.h"
#include "Assembler.h"

#include "BigMoney.h"

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
};

const size_t COMMANDNAME_MAX = 32; 

bool IsReg(char* buf);

const char CommandNames[32][COMMANDNAME_MAX] = 
{"", "push", "", "add", "sub", "div", "mul", "pow",
 "sqrt", "sin", "", "pop", "", "", "", "", "out", "", 
 "", "", "jmp", "jmb", "jme", "jmn", "", "", "", "",
 "", "", "", "hlt"};

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

int main(int argc, char* argv[])
{
    Assembly Asm = {};
    Asm.files = CmdOpenFile(argc, argv); // RENAME LATER
    
    if((Asm.LTable.labAr = LTCtor(Asm.LTable.labAr, LTLENGTH_MAX)) == NULL)
    {
        
        return LTABLE_CREATION_ERROR;
    }

    if(StackInit(&Asm.LStack, LTLENGTH_MAX, sizeof(int)) != 0)
    {
        return STACK_CREATION_ERROR;
    }
    

    if(Asm.files.err)
    {
        fprintf(stderr, "Can't open source or create object file.\n");
        return FOPEN_ERROR;
    }
    
    if(assembly(&Asm) != 0)
    {
        
        return ASSEMBLER_ERROR;
    }

    if(WriteBin(Asm.files.object, Asm.sheet.buf, Asm.sheet.size) != 0)
    {
        return BINARY_WRITING_ERROR;
    }

    LTDtor(Asm.LTable.labAr);
    fclose(Asm.files.object);
    fclose(Asm.files.source);
    return 0;
}



int assembly(Assembly* Asm)
{
    
    size_t size = GetFileSize(Asm->files.source); // default option doesn't work FIX ME
    
    int* cmdSheet = (int*)calloc(size/2, sizeof(int));
    if(cmdSheet == NULL)
    {
        return CALLOC_ERROR;
    }

    int cycleIndex = 0;

    while(ftell(Asm->files.source) < size)
    {
        
        char buffer[COMMANDNAME_MAX] = {};  
        if(fscanf(Asm->files.source, "%s", buffer) == 0)
        {
            return READING_ERROR;
        }
        char* lmarker = NULL;
        
        if((lmarker  = strchr(buffer, ':')) != NULL)
        {
            
            *lmarker = '\0';
            
            strcpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer);
            
            Asm->LTable.labAr[Asm->LTable.lnum].ipTarg = cycleIndex;
            Asm->LTable.lnum++;
            continue;
        }
        
        int command = RecogniseCommand(buffer);

        ON_DEBUG(fprintf(stderr, "## Recognized command: [%d]\n", command));

        if(command == 1)
        {
            if(fscanf(Asm->files.source, "%s", buffer) == 0)
            {
                return READING_ERROR;
            } 

            if(IsReg(buffer))
            {
                cmdSheet[cycleIndex] = PUSHR;
                cmdSheet[cycleIndex + 1] = atoi(buffer);
                cycleIndex += 2;
                continue;
            }

            cmdSheet[cycleIndex] = command;
            cmdSheet[cycleIndex + 1] = atoi(buffer);
            cycleIndex += 2;
        }

        else if(command == JMP || command == JMB || command ==  JMA || command == JME || command == JMN || command == JMBE || command == JMAE)
        {
            cmdSheet[cycleIndex] = command;
            if(fscanf(Asm->files.source, "%s", buffer) == 0)
            {
                return READING_ERROR;
            }

            if(Asm->LTable.lnum > 0)
            {
                int lab = 0;
                
                if((lab = FindLabel(&Asm->LTable, buffer)) == -1)
                {
                    return UNDEFINED_LABEL;
                }
                
                cmdSheet[cycleIndex + 1] = lab;
                cycleIndex += 2;
                continue;
            }

            else
            {
                cmdSheet[cycleIndex] = command;
                cmdSheet[cycleIndex + 1] = atoi(buffer);
                cycleIndex += 2;
                continue; // MIGHT DELETE, ADDED FOR BETTER VISION
            }
        }

        else
        {
            cmdSheet[cycleIndex] = command;
            cycleIndex++;
        }
    }


    #ifndef NDEBUG
    fprintf(stderr, "\n\n ## CMDSheet out: \n");
    for(size_t i = 0; i < cycleIndex; i++)
    {
        fprintf(stderr, "## %d\n", cmdSheet[i]);
    }
    #endif // NDEBUG3

    Asm->sheet.buf = cmdSheet;
    Asm->sheet.size = cycleIndex;

    return 0;

}

int WriteBin(FILE* obj, int* cmd, uint32_t size)
{
    uint32_t headtofile[sizeofHeader] = {sign, version, size, 0};
    fwrite(headtofile, sizeof(int), sizeofHeader, obj);
    fwrite(cmd, sizeof(int), size, obj);
    return 0;
}


bool IsReg(char* buf)
{
    ON_DEBUG(fprintf(stderr, "## PushBuf: %s\n", buf));
    if(strncmp(buf, REG_NAMES[AX], 2) == 0)
    {
        return true;
    }
    else if(strcmp(buf, REG_NAMES[BX]) == 0)
    {
        return true;
    }
    else if(strcmp(buf, REG_NAMES[CX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[DX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[EX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[FX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[GX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[HX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[IX]) == 0)
    {
        
        return true;
    }
    else if(strcmp(buf, REG_NAMES[NX]) == 0)
    {
        
        return true;
    }
    else
    {
        return false;
    }
}

int RecogniseCommand(char* buffer)
{
    if(strcmp(buffer, CommandNames[PUSH]) == 0)
    {
        return PUSH;
    }
    else if(strcmp(buffer, CommandNames[POP]) == 0)
    {
        return POP;
    }
    else if(strcmp(buffer, CommandNames[ADD]) == 0)
    {
        return ADD;
    }
    else if(strcmp(buffer, CommandNames[SUB]) == 0)
    {
        return SUB;
    }
    else if(strcmp(buffer, CommandNames[DIV]) == 0)
    {
        return DIV;
    }
    else if(strcmp(buffer, CommandNames[MUL]) == 0)
    {
        return MUL;
    }
    else if(strcmp(buffer, CommandNames[POW]) == 0)
    {
        return POW;
    }
    else if(strcmp(buffer, CommandNames[SQRT]) == 0)
    {
        return SQRT;
    }
    else if(strcmp(buffer, CommandNames[SIN]) == 0)
    {
        return SIN;
    }   
    else if(strcmp(buffer, "meow") == 0)
    {
        return MEOW;
    }                
    else if(strcmp(buffer, CommandNames[OUT]) == 0)
    {
        return OUT;
    }    
    else if(strcmp(buffer, CommandNames[JMP]) == 0)
    {
        return JMP;
    }
    else if(strcmp(buffer, CommandNames[JMBE]) == 0)
    {
        return JMBE;
    }
    else if(strcmp(buffer, CommandNames[JMAE]) == 0)
    {
        return JMAE;
    }
    else if(strcmp(buffer, CommandNames[JMA]) == 0)
    {
        return JMA;
    }
    else if(strcmp(buffer, CommandNames[JMB]) == 0)
    {
        return JMB;
    }
    else if(strcmp(buffer, CommandNames[JMN]) == 0)
    {
        return JMN;
    }
    else if(strcmp(buffer, CommandNames[JME]) == 0)
    {
        return JME;
    }
    else if(strcmp(buffer, CommandNames[31]) == 0)
    {
        return HLT;
    }        
    else 
        return 0;
}



struct Files CmdOpenFile(int c, char** v)
{
    FILE* source = NULL;
    FILE* object = NULL;
    struct Files fl = {};
    if(c > 2)
    {
        source = fopen(v[1], "r+b");
        object = fopen(v[2], "w+b");
        if(source == NULL)
        {
            fl.err = true;
            return fl;
        }
        if(object == NULL)
        {
            fl.err = true;
            return fl;
        }

        fl.source = source;
        fl.object = object;
    }

    else if(c > 1)
    {
        source = fopen(v[1], "r+b");
        object = fopen("object", "w+b");
        if(source == NULL)
        {
            fl.err = true;
            return fl;
        }
        if(object == NULL)
        {
            fl.err = true;
            return fl;
        }

        fl.source = source;
        fl.object = object;
    }

    else
    {
        source = fopen("commands", "r+b");
        if(source == NULL)
        {
            fl.err = true;
            return fl;
        }
        object = fopen("object", "r+b");
        if(object == NULL)
        {
            fl.err = true;
            return fl;
        }

    }

    return fl;
}