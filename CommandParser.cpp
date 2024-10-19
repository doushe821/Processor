#include "Assembler.h"

int ParsePop(Assembly* Asm, char* buffer, int* cycleIndex, int** cmdSheet)
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
            (*cmdSheet)[*cycleIndex] = RAM_REG_CONSTVAL;
            (*cycleIndex)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            (*cmdSheet)[*cycleIndex] = rg;
            (*cycleIndex)++;
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            (*cmdSheet)[*cycleIndex] = atoi(buffer + (eptr - plusptr + 1));
            (*cycleIndex)++;
        }

        else if((rg = FindReg(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            (*cmdSheet)[*cycleIndex] = RAM_REG;
            (*cycleIndex)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            (*cmdSheet)[*cycleIndex] = rg;
            (*cycleIndex)++;
        }

        else 
        {
            (*cmdSheet)[*cycleIndex] = RAM_CONSTVAL;
            (*cycleIndex)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            (*cmdSheet)[*cycleIndex] = atoi(buffer + 1);
            (*cycleIndex)++;
        }
    }

    else
    {
        if((rg = FindReg(buffer)) == -1)
        {
            return SYNTAX_ERROR;
        }

        (*cmdSheet)[*cycleIndex] = REG;
        (*cycleIndex)++;
        (*cmdSheet)[*cycleIndex] = rg;
        (*cycleIndex)++;
    }

    return 0;
}

int ParsePush(Assembly* Asm, char* buffer, int* cycleIndex, int** cmdSheet)
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
            (*cmdSheet)[*cycleIndex] = RAM_REG_CONSTVAL;
            (*cycleIndex)++;
            
            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            (*cmdSheet)[*cycleIndex] = rg;
            (*cycleIndex)++;
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            (*cmdSheet)[*cycleIndex] = atoi(buffer + (eptr - plusptr + 1));
            (*cycleIndex)++;
        }

        else if((rg = FindReg(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            (*cmdSheet)[*cycleIndex] = RAM_REG;
            (*cycleIndex)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            (*cmdSheet)[*cycleIndex] = rg;
            (*cycleIndex)++;
        }
        else 
        {
            (*cmdSheet)[*cycleIndex] = RAM_CONSTVAL;
            (*cycleIndex)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            (*cmdSheet)[*cycleIndex] = atoi(buffer + 1);
            (*cycleIndex)++;
        }
    }
    else if((plusptr = strchr(buffer, '+')) != NULL)
    {
        (*cmdSheet)[*cycleIndex] = REG_CONSTVAL;
        (*cycleIndex)++;

        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG_CONSTVAL));

        if((rg = FindReg(buffer)) == -1)
        {
            fprintf(stderr, "SYNTAX ERROR\n");
            return SYNTAX_ERROR;
        }

        (*cmdSheet)[*cycleIndex] = rg;
        (*cycleIndex)++;

        (*cmdSheet)[*cycleIndex] = atoi(buffer + (plusptr - buffer) + 1);
        (*cycleIndex)++; 

    }
    else if((rg = FindReg(buffer)) != -1)
    {
        (*cmdSheet)[*cycleIndex] = REG;
        (*cycleIndex)++;

        //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG));

        (*cmdSheet)[*cycleIndex] = rg;
        (*cycleIndex)++;
    }
    else
    {
        (*cmdSheet)[*cycleIndex] = CONSTVAL;
        (*cycleIndex)++;
        (*cmdSheet)[*cycleIndex] = atoi(buffer);
        (*cycleIndex)++;
        
        //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", CONSTVAL));
    }
    return 0;
}

int ParseRet(Assembly* Asm, int* cycleIndex, int** cmdSheet)
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

int ParseDraw(Assembly* Asm, int* cycleIndex, int** cmdSheet)
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

int ParseJump(Assembly* Asm, int** cmdSheet, char* buffer, int* cycleIndex)
{
    ON_DEBUG(fprintf(stderr, "## JMP ARG: %s\n", buffer));
            
    if(isdigit(buffer[0]))
    {
        *cmdSheet[*cycleIndex + 1] = atoi(buffer);
        ON_DEBUG(fprintf(stderr, "## JMP ARG ADDED: %d\n", atoi(buffer)));
        *cycleIndex += 2;                        // JUMP CODE AND ITS ARGUMENT
        return 0;
    }
            
    else
    {
        int labVal = 0;
                
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1) // LABEL IS NOT IN THE TABLE YET
        {
            strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
            Asm->LTable.lnum++;
            *cycleIndex += 2;   // JUMP CODE AND ITS ARGUMENT
            ON_DEBUG(fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            return 0;
        }

        else
        {
            (*cmdSheet)[*cycleIndex + 1] = Asm->LTable.labAr[labVal].ipTarg;
            *cycleIndex += 2;                    // JUMP CODE AND ITS ARGUMENT
            return 0;
        }
    }
}

int ParseLabel(Assembly* Asm, char* lmarker, char* buffer, int cycleIndex)
{
        int labVal = 0;
        *lmarker = '\0';
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1)
        {
            strcpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer);
            Asm->LTable.labAr[Asm->LTable.lnum].ipTarg = cycleIndex;
            Asm->LTable.lnum++;

            ON_DEBUG(fprintf(stderr, "## LABEL ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            ON_DEBUG(fprintf(stderr, "## READ: %s\n", buffer));
        }

        else if(Asm->LTable.labAr[labVal].ipTarg == -1)
        {

            Asm->LTable.labAr[labVal].ipTarg = cycleIndex; 

            ON_DEBUG(fprintf(stderr, "## LABEL VALUE ADDED: %s = %d\n", buffer,Asm->LTable.labAr[labVal].ipTarg));
            ON_DEBUG(LTDump(&Asm->LTable));

        }   
        else
        {

        }
        
        return 0;
}