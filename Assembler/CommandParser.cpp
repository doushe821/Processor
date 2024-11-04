#include "Assembler.h"

int ParsePop(Assembly_t* Asm, char* buffer)
{
    char* cptr = NULL;
    char* eptr = NULL;
    char* plusptr = NULL;
    char* regArg = NULL;
    char argBuffer[ARGLEN_MAX] = {};
    uint32_t rg = 0;

    uint8_t tempByte = 0;
    double tempVal = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            tempByte = RAM_REG_CONSTVAL;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == UINT32_MAX - 1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            VStackPush(Asm->cmdStack, &rg, sizeof(rg));
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            uint32_t ramNum = atoi(buffer + (eptr - plusptr + 1));
            VStackPush(Asm->cmdStack, &ramNum, sizeof(ramNum));
            
        }

        else if((rg = FindReg(buffer + 1)) != UINT32_MAX - 1) // buffer + (cptr - buffer)
        {
            tempByte = RAM_REG;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            VStackPush(Asm->cmdStack, &rg, sizeof(rg));
            
        }

        else 
        {
            tempByte = RAM_CONSTVAL;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            uint32_t ramNum = atoi(buffer + 1);
            VStackPush(Asm->cmdStack, &ramNum, sizeof(ramNum));
            
        }
    }

    else
    {
        if((rg = FindReg(buffer)) == UINT32_MAX - 1)
        {
            return SYNTAX_ERROR;
        }

        tempByte = REG;
        VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
        
        VStackPush(Asm->cmdStack, &rg, sizeof(rg));
        
    }

    return 0;
}

int ParsePush(Assembly_t* Asm, char* buffer)
{
    uint8_t tempByte = 0;
    double tempVal = 0;

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
            tempByte = RAM_REG_CONSTVAL;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            
            
            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((rg = FindReg(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            VStackPush(Asm->cmdStack, &rg, sizeof(rg));
            
            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            uint32_t ramNum = atoi(buffer + (eptr - plusptr + 1));
            VStackPush(Asm->cmdStack, &ramNum, sizeof(ramNum));
            
        }

        else if((rg = FindReg(buffer + 1)) != UINT32_MAX - 1) // buffer + (cptr - buffer)
        {
            tempByte = RAM_REG;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));    

            VStackPush(Asm->cmdStack, &rg, sizeof(rg));
            
        }
        else 
        {
            tempByte = RAM_CONSTVAL;
            VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
            

            ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));
            
            uint32_t ramCNum = atoi(buffer + 1);

            VStackPush(Asm->cmdStack, &ramCNum, sizeof(ramCNum));
            
        }
    }
    else if((rg = FindReg(buffer)) != UINT32_MAX - 1)
    {
        tempByte = REG;
        VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
        

        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", REG));

        VStackPush(Asm->cmdStack, &rg, sizeof(rg));
    }
    else
    {
        tempByte = CONSTVAL;
        VStackPush(Asm->cmdStack, &tempByte, sizeof(tempByte));
        double cVal = atof(buffer);
        VStackPush(Asm->cmdStack, &cVal, sizeof(cVal));
        
        ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", CONSTVAL));
    }
    return 0;
}

int ParseSleep(Assembly_t* Asm, char* buffer)
{

    uint32_t time = atoi(buffer);
    VStackPush(Asm->cmdStack, &time, sizeof(time));

    return 0;
}

int ParseDraw(Assembly_t* Asm, char* buffer)
{
    char* divisor = strchr(buffer, 'x');   
    if(divisor == NULL)
    {
        return SYNTAX_ERROR;
    }     

    uint32_t width  = atoi(buffer);
    VStackPush(Asm->cmdStack, &width, sizeof(width));

    uint32_t height = atoi(divisor + 1);
    VStackPush(Asm->cmdStack, &height, sizeof(height));
    
    return 0;
}

int ParseJump(Assembly_t* Asm, char* buffer)
{
    ON_DEBUG(fprintf(stderr, "## JMP ARG: %s\n", buffer));
            
    if(isdigit(buffer[0]))
    {
        uint32_t jumpIP =  atoi(buffer);
        VStackPush(Asm->cmdStack, &jumpIP, sizeof(jumpIP));
        
        ON_DEBUG(fprintf(stderr, "## JMP ARG ADDED: %d\n", atoi(buffer)));              
        return 0;

    }
            
    else
    {
        uint32_t labVal = 0;
                
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1)                           // LABEL IS NOT IN THE TABLE YET
        {
            strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
            Asm->LTable.lnum++;
            ON_DEBUG(fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer));
            ON_DEBUG(LTDump(&Asm->LTable));
            VStackPush(Asm->cmdStack, &labVal, 4);
            return 0;
        }

        else
        {

            ON_DEBUG(fprintf(stderr, "$$$ %lu\n", sizeof(Asm->LTable.labAr[labVal].ipTarg)));
            ON_DEBUG(fprintf(stderr, "$$$ IP TARGET = %u\n", Asm->LTable.labAr[labVal].ipTarg));
            
            VStackPush(Asm->cmdStack, &Asm->LTable.labAr[labVal].ipTarg, sizeof(Asm->LTable.labAr[labVal].ipTarg));
            
            return 0;
        }
    }
}

int ParseNoArg(Assembly_t* Asm, char* buffer)
{
    return 0;
}

int ParseCall(Assembly_t* Asm, char* buffer)
{
    ON_DEBUG(fprintf(stderr, "## CALL ARG: %s\n", buffer));
            
    uint32_t labVal = 0;
            
    if((labVal = FindLabel(&Asm->LTable, buffer)) == -1) // LABEL IS NOT IN THE TABLE YET
    {
        strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
        Asm->LTable.lnum++;
        ON_DEBUG(fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer));
        ON_DEBUG(LTDump(&Asm->LTable));
        VStackPush(Asm->cmdStack, &labVal, sizeof(labVal));
        return 0;
    }

    else
    {

        ON_DEBUG(fprintf(stderr, "$$$ %lu\n", sizeof(Asm->LTable.labAr[labVal].ipTarg)));
        ON_DEBUG(fprintf(stderr, "$$$ IP TARGET = %u\n", Asm->LTable.labAr[labVal].ipTarg));
        
        VStackPush(Asm->cmdStack, &Asm->LTable.labAr[labVal].ipTarg, sizeof(Asm->LTable.labAr[labVal].ipTarg));
        
        return 0;
    }
    
}

int ParseUnknown(Assembly_t* Asm, char* buffer)
{
    fprintf(stderr, "Unknown command\n");
    return SYNTAX_ERROR;
}

int ParseLabel(Assembly_t* Asm, char* buffer, size_t cmdIndex, char* lmarker)
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
    return UINT32_MAX - 1;
}