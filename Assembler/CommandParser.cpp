#include "Assembler.h"

int ParsePop(Assembly* Asm, char* buffer, int* cmdIndex)
{
    char* cptr = NULL;
    char* eptr = NULL;
    char* plusptr = NULL;
    char* regArg = NULL;
    char argBuffer[ARGLEN_MAX] = {};
    uint32_t rg = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            Asm->sheet.buf[*cmdIndex] = RAM_REG_CONSTVAL;
            (*cmdIndex)++;

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
            (*cmdIndex) += sizeof(rg);
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            uint32_t ramNum = atoi(buffer + (eptr - plusptr + 1));
            memcpy(Asm->sheet.buf + *cmdIndex, &ramNum, sizeof(ramNum));
            (*cmdIndex) += sizeof(ramNum);
        }

        else if((rg = FindReg(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            Asm->sheet.buf[*cmdIndex] = RAM_REG;
            (*cmdIndex)++;

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
            (*cmdIndex) += sizeof(rg);
        }

        else 
        {
            Asm->sheet.buf[*cmdIndex] = RAM_CONSTVAL;
            (*cmdIndex)++;

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            uint32_t ramNum = atoi(buffer + 1);
            memcpy(Asm->sheet.buf + *cmdIndex, &ramNum, sizeof(ramNum));
            (*cmdIndex) += sizeof(ramNum);
        }
    }

    else
    {
        if((rg = FindReg(buffer)) == -1)
        {
            return SYNTAX_ERROR;
        }

        Asm->sheet.buf[*cmdIndex] = REG;
        (*cmdIndex)++;
        memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
        (*cmdIndex) += sizeof(rg);
    }

    return 0;
}

int ParsePush(Assembly* Asm, char* buffer, int* cmdIndex)
{

    char* cptr = NULL;
    char* eptr = NULL;
    char* plusptr = NULL;
    char* regArg = NULL;
    char argBuffer[ARGLEN_MAX] = {};
    uint32_t rg = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            Asm->sheet.buf[*cmdIndex] = RAM_REG_CONSTVAL;
            (*cmdIndex)++;
            
            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(uint32_t));
            (*cmdIndex) += sizeof(uint32_t);
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            uint32_t ramNum = atoi(buffer + (eptr - plusptr + 1));
            memcpy(Asm->sheet.buf + *cmdIndex, &ramNum, sizeof(uint32_t));
            (*cmdIndex) += sizeof(ramNum);
        }

        else if((rg = FindReg(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            Asm->sheet.buf[*cmdIndex] = RAM_REG;
            (*cmdIndex)++;

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
            (*cmdIndex) += sizeof(rg);
        }
        else 
        {
            Asm->sheet.buf[*cmdIndex] = RAM_CONSTVAL;
            (*cmdIndex)++;

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            uint32_t ramCNum = atoi(buffer + 1);

            memcpy(Asm->sheet.buf + *cmdIndex, &ramCNum, sizeof(ramCNum));
            (*cmdIndex) += sizeof(ramCNum);
        }
    }
    else if((plusptr = strchr(buffer, '+')) != NULL)
    {
        Asm->sheet.buf[*cmdIndex] = REG_CONSTVAL;
        (*cmdIndex)++;

        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG_CONSTVAL));

        if((rg = FindReg(buffer)) == -1)
        {
            fprintf(stderr, "SYNTAX ERROR\n");
            return SYNTAX_ERROR;
        }

        memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
        (*cmdIndex) += sizeof(rg);


        uint32_t ramNum = atoi(buffer + (plusptr - buffer) + 1);
        memcpy(Asm->sheet.buf + *cmdIndex, &ramNum, sizeof(ramNum));
        (*cmdIndex) += sizeof(ramNum); 

    }
    else if((rg = FindReg(buffer)) != -1)
    {
        Asm->sheet.buf[*cmdIndex] = REG;
        (*cmdIndex)++;

        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG));

        memcpy(Asm->sheet.buf + *cmdIndex, &rg, sizeof(rg));
        (*cmdIndex) += sizeof(rg);
    }
    else
    {
        Asm->sheet.buf[*cmdIndex] = CONSTVAL;
        (*cmdIndex)++;
        double cVal = atoi(buffer);
        memcpy(Asm->sheet.buf + *cmdIndex, &cVal, sizeof(cVal));
        (*cmdIndex) += sizeof(cVal);
        
        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", CONSTVAL));
    }
    return 0;
}

int ParseRet(Assembly* Asm, int* cycleIndex, char** cmdSheet)
{
    *cmdSheet[*cycleIndex] = JMP;
    (*cycleIndex)++;    
    uint32_t addr = 0;
    StackPop(Asm->LStack, &addr);
    *cmdSheet[*cycleIndex] = addr;
    (*cycleIndex)++;
    return 0;
}

int ParseSleep(Assembly* Asm, int* cycleIndex, int** cmdSheet)
{
    *cmdSheet[*cycleIndex] = SLEEP;
    (*cycleIndex)++;

    char buffer[COMMANDNAME_MAX] = {};

    if(fscanf(Asm->files.source, "%s", buffer) == 0)
    {
        return READING_ERROR;
    }


    *cmdSheet[*cycleIndex] = atoi(buffer);    
    (*cycleIndex)++;

    return 0;
}

int ParseDraw(Assembly* Asm, int* cycleIndex, char** cmdSheet)
{
    *cmdSheet[*cycleIndex] = DRAW;
    (*cycleIndex)++;

    char buffer[COMMANDNAME_MAX] = {};

    if(fscanf(Asm->files.source, "%s", buffer) == 0)
    {
        return READING_ERROR;
    }

    *cmdSheet[*cycleIndex] = atoi(buffer);
    (*cycleIndex)++;

    if(fscanf(Asm->files.source, "%s", buffer) == 0)
    {
        return READING_ERROR;
    }

    *cmdSheet[*cycleIndex] = atoi(buffer);
    (*cycleIndex)++;

    if(fscanf(Asm->files.source, "%s", buffer) == 0)
    {
        return READING_ERROR;
    }
            
    return 0;
}

int ParseJump(Assembly* Asm, char* buffer, int* cmdIndex)
{
    ON_DEBUG(fprintf(stderr, "## JMP ARG: %s\n", buffer));
            
    if(isdigit(buffer[0]))
    {

        uint32_t jumpIP =  atoi(buffer);
        memcpy(Asm->sheet.buf + *cmdIndex, &jumpIP, sizeof(jumpIP));
        *cmdIndex += sizeof(jumpIP);
        
        ON_DEBUG(fprintf(stderr, "## JMP ARG ADDED: %d\n", atoi(buffer)));              
        return 0;

    }
            
    else
    {
        uint32_t labVal = 0;
                
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1) // LABEL IS NOT IN THE TABLE YET
        {
            strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
            *cmdIndex += sizeof(Asm->LTable.labAr[Asm->LTable.lnum].ipTarg);
            Asm->LTable.lnum++;
            ON_DEBUG(fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            return 0;
        }

        else
        {

            ON_DEBUG(fprintf(stderr, "$$$ %lu\n", sizeof(Asm->LTable.labAr[labVal].ipTarg)));
            ON_DEBUG(fprintf(stderr, "$$$ IP TARGET = %u\n", Asm->LTable.labAr[labVal].ipTarg));
            
            memcpy(Asm->sheet.buf + *cmdIndex, &Asm->LTable.labAr[labVal].ipTarg, sizeof(Asm->LTable.labAr[labVal].ipTarg));
            (*cmdIndex) += sizeof(Asm->LTable.labAr[labVal].ipTarg);
            return 0;
        }
    }
}

int ParseLabel(Assembly* Asm, char* buffer, size_t cmdIndex, char* lmarker)
{
        int labVal = 0;
        *lmarker = '\0';
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1)
        {
            strcpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer);
            Asm->LTable.labAr[Asm->LTable.lnum].ipTarg = cmdIndex;
            fprintf(stderr, "$$$$$ %u\n", Asm->LTable.labAr[Asm->LTable.lnum].ipTarg);
            Asm->LTable.lnum++;

            ON_DEBUG(fprintf(stderr, "## LABEL ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            ON_DEBUG(fprintf(stderr, "## READ: %s\n", buffer));
        }

        else if(Asm->LTable.labAr[labVal].ipTarg == -1)
        {

            Asm->LTable.labAr[labVal].ipTarg = cmdIndex; 

            ON_DEBUG(fprintf(stderr, "## LABEL VALUE ADDED: %s = %d\n", buffer,Asm->LTable.labAr[labVal].ipTarg));
            ON_DEBUG(LTDump(&Asm->LTable));

        }   
        else
        {

        }
        
        return 0;
}