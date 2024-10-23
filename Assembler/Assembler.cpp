#include "Assembler.h"

int main(int argc, char* argv[])
{
    Assembly Asm = {};
    Asm.files = CmdOpenFile(argc, argv);  // SOURCE OBJECT OPENED
    
    if((Asm.LTable.labAr = LTCtor(Asm.LTable.labAr, LTLENGTH_MAX)) == NULL) // LT CREATED
    {
        
        return LTABLE_CREATION_ERROR;
    }
    $;
    if(StackInit(&Asm.LStack, LTLENGTH_MAX, sizeof(int)) != 0) // MAKE IT SQUIRT NO DOUBLE NEEDED
    {
        return STACK_CREATION_ERROR;
    }
    $;

    if(Asm.files.err)
    {
        fprintf(stderr, "Can't open source or create object file.\n");
        return FOPEN_ERROR;
    }
    $;
    if(Bufferize(&Asm) != 0)
    {
        return READING_ERROR;
    }

    $;
    fclose(Asm.files.source); // SOURCE CLOSED
    $;
    if(assembly(&Asm) != 0)
    {  
        fprintf(stderr, "First assembling error\n");
        return ASSEMBLER_ERROR;
    }

    $;
    ON_DEBUG(fprintf(stderr, "FSDFKLDJKSLKDSJ LNUM: %zu\n", Asm.LTable.lnum));

    if(Asm.LTable.lnum > 0)
    {
        $;
        free(Asm.sheet.buf); // HAVE TO CLEAN CODE 
        $; 
        
        //ON_DEBUG(LTDump(&Asm.LTable));

        if(assembly(&Asm) != 0)
        {
            fprintf(stderr, "Second assembling error\n");
            return ASSEMBLER_ERROR;
        }
    }
    $;

    /*for(size_t i = 0; i < Asm.sheet.size; i++)
    {
        fprintf(stderr, "%c\n", Asm.sheet.buf[i]);
    }*/
    
    if(WriteBin(Asm.files.object, Asm.sheet.buf, Asm.sheet.size) != 0)
    {
        return BINARY_WRITING_ERROR;
    }
    $;

    free(Asm.sheet.buf);
    $;
    LTDtor(Asm.LTable.labAr); // LT DESTROYED
    $;
    fclose(Asm.files.object); // OBJECT CLOSED
    $;
    return 0;
}


int assembly(Assembly* Asm)
{
    
    int cmdIndex = 0;
    size_t bread = 0;

    while(bread < Asm->SourceSize)
    {
        ON_DEBUG(fprintf(stderr, "## IP (CMD INDEX) = %d\n", cmdIndex));
        ON_DEBUG(LTDump(&Asm->LTable));

        char buffer[COMMANDNAME_MAX] = {}; 

        if(GetCommand(Asm, buffer, &bread) != 0)
        {
            fprintf(stderr, "nigga cant read\n");
            return READING_ERROR;
        }
        ON_DEBUG(fprintf(stderr, "%s\n", buffer));

        char* lmarker = NULL;
        if((lmarker  = strchr(buffer, ':')) != NULL)
        {

            ParseLabel(Asm, buffer, cmdIndex, lmarker);

            size_t blen = strlen(buffer);
            for(size_t i = 0; i < blen; i++)
            {
                buffer[i] = '\0';
            }

            ON_DEBUG(fprintf(stderr, "6969696996 %s\n", buffer));

            if(GetCommand(Asm, buffer, &bread) != 0)
            {
                return READING_ERROR;
            }

        }
        
        int command = RecogniseCommand(buffer);

        ON_DEBUG(fprintf(stderr, "## Recognized command: [%d]\n", command));

        if(command == PUSH)
        {
            Asm->sheet.buf[cmdIndex] = (char)PUSH; // HAVE TO CAST TO CONTINUE USING ENUm
            cmdIndex++;

            if(GetCommand(Asm, buffer, &bread) != 0)
            {
                return READING_ERROR;
            }

            ParsePush(Asm, buffer, &cmdIndex);
        }

        else if(command == POP)
        {
            Asm->sheet.buf[cmdIndex] = (char)POP;
            cmdIndex++;

            if(GetCommand(Asm, buffer, &bread) != 0)
            {
                return READING_ERROR;
            }
    
            ParsePop(Asm, buffer, &cmdIndex);
        }

        else if(command == JMP || command == JMB || command ==  JMA || command == JME || 
                command == JMN || command == JMBE || command == JMAE)
        {
            Asm->sheet.buf[cmdIndex] = command;
            cmdIndex++;

            if(GetCommand(Asm, buffer, &bread) != 0)
            {
                return READING_ERROR;
            }

            ParseJump(Asm, buffer, &cmdIndex);
            
        }
        else if(command != 0)
        {
            Asm->sheet.buf[cmdIndex] = command;
            cmdIndex++;
        }
        else
        {
            cmdIndex++;
        }
    }

    Asm->sheet.size = cmdIndex;
    /*#ifndef NDEBUG
    fprintf(stderr, "\n\n## Asm->sheet.buf out: \n");
    for(size_t i = 0; i < cycleIndex; i++)
    {
        fprintf(stderr, "## %d\n", Asm->sheet.buf[i]);
    }
    #endif // NDEBUG3 */
    return 0;

}

int WriteBin(FILE* obj, char* cmd, uint32_t size)
{
    uint32_t headtofile[sizeofHeader] = {sign, version, size, 0};
    fwrite(headtofile, sizeof(int), sizeofHeader, obj);
    fwrite(cmd, sizeof(char), size, obj);
    return 0;
}


int GetCommand(Assembly* Asm, char* buffer, size_t* bread)
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
    fprintf(stderr, "7777777777777777%s\n", buffer);
    *bread += commandptr - (Asm->Source + *bread) + 1; // +1 SO NEXT STRCHR DOESN'T INCLUDE LAST SEPARATION MARK
    fprintf(stderr, "$$ BREAD = %zu\n", *bread);
    return 0;
}

uint32_t FindReg(char* buf)
{
    //ON_DEBUG(fprintf(stderr, "  ARG BUF: %s\n", buf));
    if(strncmp(buf, REG_NAMES[AX], 2) == 0)
    {
        return AX;
    }
    else if(strncmp(buf, REG_NAMES[BX], 2) == 0)
    {
        return BX;
    }
    else if(strncmp(buf, REG_NAMES[CX], 2) == 0)
    {
        return CX;
    }
    else if(strncmp(buf, REG_NAMES[DX], 2) == 0)
    {
        return DX;
    }
    else if(strncmp(buf, REG_NAMES[EX], 2) == 0)
    {
        return EX;
    }
    else if(strncmp(buf, REG_NAMES[FX], 2) == 0)
    {   
        return FX;
    }
    else if(strncmp(buf, REG_NAMES[GX], 2) == 0)
    {
        
        return GX;
    }
    else if(strncmp(buf, REG_NAMES[HX], 2) == 0)
    {   
        return HX;
    }
    else if(strncmp(buf, REG_NAMES[IX], 2) == 0)
    {   
        return IX;
    }
    else if(strncmp(buf, REG_NAMES[NX], 2) == 0)
    {   
        return NX;
    }
    else
    {
        return -1;
    }
}

int RecogniseCommand(char* buffer)
{
    fprintf(stderr, "## CMD BUFFER : %s\n", buffer);
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
    else if(strcmp(buffer, CommandNames[RET]) == 0)
    {
        return RET;
    }
    else if(strcmp(buffer, CommandNames[SLEEP]) == 0)
    {
        return SLEEP;

    }
    else if(strcmp(buffer, CommandNames[DRAW]) == 0)
    {
        return DRAW;

    }
    else if(strcmp(buffer, CommandNames[HLT]) == 0)
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


int Bufferize(Assembly* Asm)
{
    Asm->SourceSize = GetFileSize(Asm->files.source); // default option doesn't work FIX ME
    $;
    fprintf(stderr, "Soource size = %zu\n", Asm->SourceSize);
    Asm->Source = (char*)calloc(Asm->SourceSize, sizeof(char));
    $;
    if(fread(Asm->Source, Asm->SourceSize, sizeof(char), Asm->files.source) == 0)
    {
        fprintf(stderr, "Reading error\n");
        return READING_ERROR;
    }
    $;
    for(size_t i = 0; i < Asm->SourceSize; i++)
    {
        fprintf(stderr, "i befor: %zu\n", i);
        char* nlineptr = NULL;
        if((nlineptr = strchr(Asm->Source + i, '\n')) != NULL)
        {
            *nlineptr = ' ';
            $;
            i += nlineptr - (Asm->Source + i);
        }
        fprintf(stderr, "i after: %zu\n", i);
    }
    $;

    Asm->sheet.buf = (char*)calloc(Asm->SourceSize, 2*sizeof(char));


    if(Asm->sheet.buf == NULL)
    {
        return CALLOC_ERROR;
    }

    return 0;
}
