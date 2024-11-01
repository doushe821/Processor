#include "Assembler.h"

int main(int argc, char* argv[])
{
    Assembly_t Asm = {};
    Asm.files = CmdOpenFile(argc, argv);  // SOURCE OBJECT OPENED
    
    if((Asm.LTable.labAr = LTCtor(Asm.LTable.labAr, LTLENGTH_MAX)) == NULL) // LT CREATED
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
    
    if(Bufferize(&Asm) != 0)
    {
        fprintf(stderr, "Failed to bufferise commands file\n");
        return READING_ERROR;
    }

    
    fclose(Asm.files.source); // SOURCE CLOSED
    
    if(Assembly(&Asm) != 0)
    {  
        fprintf(stderr, "First assembling error\n");
        return ASSEMBLER_ERROR;
    }


    
    ON_DEBUG(fprintf(stderr, "FSDFKLDJKSLKDSJ LNUM: %zu\n", Asm.LTable.lnum));

    if(Asm.LTable.lnum > 0)
    {
        StackDtor(Asm.cmdStack);
        VStackInit(&Asm.cmdStack, Asm.SourceSize);
        if(Assembly(&Asm) != 0)
        {
            fprintf(stderr, "Second assembling error\n");
            return ASSEMBLER_ERROR;
        }
    }
    
    if(WriteBin(Asm.files.object, Asm.cmdStack) != 0)
    {
        return BINARY_WRITING_ERROR;
    }

    LTDtor(Asm.LTable.labAr); 
    
    fclose(Asm.files.object); 
    
    return 0;
}


int Assembly(Assembly_t* Asm)
{
    size_t bread = 0;
    char* CheckPtr = NULL;

    while(bread < Asm->SourceSize)
    {
        ON_DEBUG(LTDump(&Asm->LTable));

        char buffer[COMMANDNAME_MAX] = {}; 

        if(GetCommand(Asm, buffer, &bread) != 0)
        {
            fprintf(stderr, "Can't read next command while assembling\n");
            fprintf(stderr, "Current ip: %zu\nLine: %d\n", GetStackSize(Asm->cmdStack), __LINE__);
            return READING_ERROR;
        }
        ON_DEBUG(fprintf(stderr, "%s\n", buffer));

        char* lmarker = NULL;
        if((lmarker  = strchr(buffer, ':')) != NULL)
        {

            ParseLabel(Asm, buffer, GetStackSize(Asm->cmdStack), lmarker);

            size_t blen = strlen(buffer);
            for(size_t i = 0; i < blen; i++)
            {
                buffer[i] = '\0';
            }

            if(GetCommand(Asm, buffer, &bread) != 0)
            {
                fprintf(stderr, "Can't read next command while assembling\n");
                fprintf(stderr, "Current ip: %zu\nLine: %d\n", GetStackSize(Asm->cmdStack), __LINE__);
                return READING_ERROR;
            }

        }
        
        int command = RecogniseCommand(buffer);

        if(ParseCommand(Asm, command, buffer, &bread) != 0)
        {
            return HANDLER_ERROR;
        }
    }
    return 0;

}

int WriteBin(FILE* obj, Stack_t* cmdStack)
{
    uint32_t size = GetStackSize(cmdStack);
    uint32_t headtofile[SIZE_OF_HEADER] = {H_SIGN, H_VERSION, size, 0};
    if(fwrite(headtofile, sizeof(int), SIZE_OF_HEADER, obj) != SIZE_OF_HEADER)
    {
        fprintf(stderr, "Unable to write header: %s:%d\n", __FILE__, __LINE__);
        return BINARY_WRITING_ERROR;
    }
    char* bufptr = (char*)GetDataPointer(cmdStack);
    if(fwrite(bufptr, sizeof(char), size, obj) != size)
    {
        fprintf(stderr, "Unable to write binary file: %s:%d\n", __FILE__, __LINE__);
        return BINARY_WRITING_ERROR;
    } 
    return 0;
}

int GetCommand(Assembly_t* Asm, char* buffer, size_t* bread)
{
    char* commandptr = NULL;

    if((commandptr = strchr(Asm->Source + *bread, ' ')) == NULL)
    {
        return READING_ERROR;
    }

    if(commandptr - (Asm->Source + *bread) > COMMANDNAME_MAX)
    {
        return SYNTAX_ERROR;
    }
    char bbb[COMMANDNAME_MAX] = {};
    strncpy(buffer, Asm->Source + *bread, commandptr - (Asm->Source + *bread));
    *bread += commandptr - (Asm->Source + *bread) + 1; // +1 SO NEXT STRCHR DOESN'T INCLUDE LAST SEPARATION MARK
    return 0;
}

uint32_t FindReg(char* buf)
{
    //ON_DEBUG(fprintf(stderr, "  ARG BUF: %s\n", buf));
    for(int i = 0; i < NUMBER_OF_REGS; i++)
    {
        if(strncmp(buf, REG_NAMES[i], REGNAME_MAX) == 0)
        {
            return i;
        }
    }
    return -1;
}

int RecogniseCommand(char* buffer)
{
    ON_DEBUG(fprintf(stderr, "## CMD BUFFER : %s\n", buffer));
    for(int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        if(strncmp(buffer, CommandsStruct[i].Name, COMMANDNAME_MAX) == 0)
        {
            return i;
        }
    }
    return SYNTAX_ERROR;
}

int ParseCommand(Assembly_t* Asm, int code, char* buffer, size_t* bread)
{
    for(size_t i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        if(code == CommandsStruct[i].OpCode)
        {
            if(CommandsStruct[i].HasArg)
            {
                if(GetCommand(Asm, buffer, bread) != 0)
                {
                    fprintf(stderr, "Can't read next command while assembling\n");
                    fprintf(stderr, "Current ip: %zu\nLine: %d\n", GetStackSize(Asm->cmdStack), __LINE__);
                    return READING_ERROR;
                }
                VStackPush(Asm->cmdStack, &i, 1);
                CommandsStruct[i].ParseFunc(Asm, buffer);
            }
            else
            {
                VStackPush(Asm->cmdStack, &i, 1);
            }
        }
    }
    return 0;
}


struct Files CmdOpenFile(const int argc, char** argv) // argc argv
{
    FILE* source = NULL;
    FILE* object = NULL;
    struct Files fl = {};
    if(argc > 2)
    {
        source = fopen(argv[1], "r+b");
        object = fopen(argv[2], "w+b");
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

    else if(argc > 1)
    {
        source = fopen(argv[1], "r+b");
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

int Bufferize(Assembly_t* Asm)
{
    Asm->SourceSize = GetFileSize(Asm->files.source); // default option doesn't work FIX ME
    
    #ifndef NDEBUG
    fprintf(stderr, "Source size = %zu\n", Asm->SourceSize);
    #endif

    Asm->Source = (char*)calloc(Asm->SourceSize, sizeof(char));
    
    if(fread(Asm->Source, sizeof(char), Asm->SourceSize, Asm->files.source) != Asm->SourceSize) 
    {
        fprintf(stderr, "Reading error: %s:%d\n", __FILE__, __LINE__);
        return READING_ERROR;
    }
    
    for(size_t i = 0; i < Asm->SourceSize; i++)
    {
        char* nlineptr = NULL;
        if((nlineptr = strchr(Asm->Source + i, '\n')) != NULL)
        {
            *nlineptr = ' ';
            
            i += nlineptr - (Asm->Source + i);
        }
    }

    if(VStackInit(&Asm->cmdStack, Asm->SourceSize/2) != 0)
    {
        fprintf(stderr, "CMD Stack initialisation error\n");
        return STACK_CREATION_ERROR;
    }

    return 0;
}
