#include "StackDB.h"
#include "StackStruct.h"

void StackAssert(Stack_t* stk, const char* stkName, const char* file, const char* func, int line)
{
    if(stk == NULL)
    {
        
        fprintf(stderr, "Stack pointer is NULL\n");
        assert(0);
    }
    
    if((stk->Error = StackError(stk)) != 0)
    {
        
        VoidIntDump(stk);
        assert(0 && "Emergency exit, stack dumped into log.txt\n");
    }
}

int StackError(Stack_t* stk)
{
    size_t errFlag = 0;
    
    if(stk == NULL)
    {
        fprintf(stderr, "Stack pointer is NULL\n");
        errFlag += STACK_PTR_IS_NULL;
    }
    
    if(stk->data == NULL)
    {
        fprintf(stderr, "Data pointer is NULL\n");
        errFlag += DATA_PTR_IS_NULL;
    }
    
    if(stk->size > stk->capacity)
    {
        fprintf(stderr, "Stack overflow\n");
        errFlag += STACK_OVERFLOW;
    }
    
    if(stk->size > UNLIKELY_STACK_SIZE)
    {
        fprintf(stderr, "Stack has negative size (stack underflow)\n");
        errFlag += STACK_UNDERFLOW;
    }
    
    #ifndef NDEBUG
    if(*(int*)((char*)stk->data) != DataCanaryGuardBotREF)
    {
        fprintf(stderr, "Data bot canary edad\n");
        errFlag += BOT_DATA_CANARY_CORRUPTED;
    }
    
    if(*(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize) != DataCanaryGuardTopREF)
    {
        
        fprintf(stderr, "Data top canary edad\n");
        errFlag += TOP_DATA_CANARY_CORRUPTED;
    }
    
    if(stk->StructCanaryGuardBot != StructCanaryGuardBotREF)
    {
        fprintf(stderr, "Struct bot canary edad\n");
        errFlag += BOT_STACK_CANARY_CORRUPTED;
    }
    
    if(stk->StructCanaryGuardTop != StructCanaryGuardTopREF)
    {
        fprintf(stderr, "Struct top canary edad\n");
        errFlag += TOP_STACK_CANARY_CORRUPTED;
    }
    
    if(stk->DataHash != hash(stk->data, stk->capacity + 2)) 
    {
        fprintf(stderr, "Data hash edad\n");
        errFlag += DATA_CORRUPTED;
    }

    #endif
    /*if(stk->StructHash != hash(stk, sizeof(Stack_t)))
    {
        fprintf(stderr, "Struct hash edad\n");
        errFlag += STACK_STRUCT_CORRUPTED;
    }*/

    return errFlag;
}

int doStackDump(Stack_t* stk, const char* stkName, const char* file, const char* func, int line)
{
    FILE* log = fopen("log.txt", "w+b");
    if(log == NULL)
    {
        stk->Error = FILE_CREATION_ERROR;
        fclose(log);
        return FILE_CREATION_ERROR;
    }

    fprintf(log, 
    "########################## STACK INFO ##########################\n"
    "## CALLER STACK: %p\n"
    "## CALLER FILE    : %s\n"
    "## CALLER FUNCTION: %s\n"
    "## LINE          = %d\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## STACK DATA: %p\n",
    stk, file, func, line, stk->size, stk->capacity, stk->data);

    for(int i = 0; i < stk->capacity; i++)
    {
        if(*((int*)((char*)stk->data + i*stk->size*stk->elSize)) == 89898)//TODO byte comparison
        {
            fprintf(log, "*[%c] (POISON), ", *((char*)stk->data + i*stk->elSize));
        }
        else
        {
            fprintf(log, "[%c], ", *((int*)((char*)stk->data + i*stk->size*stk->elSize)));
        }

        if((i + 1) % 5 == 0)
        {
            fprintf(log, " (%d)\n", i+1);
        }
    }
    fprintf(log, "\n\n\n");
    fprintf(log, 
    "## STACK ERRORS: ");

    u_int16_t byte = 2;
    for(int i = 0; i < sizeof(u_int16_t)*8; i++)
    {
        fprintf(log, "%d", (byte & stk->Error) ? 1 : 0);
        byte *= 2;
    }
    
    fprintf(log, "\n\n\n");


    fclose(log);

    return NO_ERRORS;
}


int VoidIntDump(Stack_t* stk)
{
    
    //fprintf(stderr, "DUMP rofl\n");
    
    FILE* log = fopen("log.txt", "a+b");
    
    if(log == NULL)
    {
        
        //fprintf(stderr, "roflreading error\n");
        stk->Error = FILE_CREATION_ERROR;
        
        fclose(log);
        
        return FILE_CREATION_ERROR;
        
    }

    //fprintf(stderr, "rofl FILE READ SUCCES\n");
    
    fprintf(log, 
    "########################## STACK INFO ##########################\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## STACK DATA: %p\n", stk->size, stk->capacity, (char*)stk->data);
       
    fprintf(log, "## DATA BUFFER: \n");
    
    fprintf(log, "### BOTTOM CANARY: %d\n", *(int*)((char*)stk->data));
    
    for(size_t i = 1; i < stk->capacity + 1; i++)
    {
        
        fprintf(log, "# [%d]  (%zu)\n", *(int*)((char*)stk->data + i*stk->elSize), i);
        
    }
    
    fprintf(log, "### TOP CANARY: %d\n", *(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize));

    #ifndef NDEBUG
    fprintf(log, "##  DATA HASH SUM: %zu\n", stk->DataHash);
    fprintf(log, "##  STRUCT HASH SUM: %zu\n", stk->StructHash);
    #endif

    fprintf(log, "## ERRORS: ");
    
    uint16_t byte = 2; 
    
    for(size_t i = 0; i < sizeof(uint16_t)*8; i++)
    {
        
        fprintf(log, "%hu", (stk->Error & byte) ? 1 : 0);
        
        byte *= 2;
        
    }

    fprintf(log, "## ERROR DESCRIPTION: \n"
    "## 1) Undefined error, report to developer, please\n"
    "## 2) Memory allocation error.\n"
    "## 3) Destructor failed.\n"
    "## 4) Log file cannot be created.\n"
    "## 5) memset() failed.\n"
    "## 6) Stack underflow.\n"
    "## 7) Stack overflow.\n"
    "## 8) Data pointer is null.\n"
    "## 9) Stack pointer is null.\n"
    "## 10) Bot data canary is corrupted.\n"
    "## 11) Top data canary is corrupted.\n"
    "## 12) Bot structure canary is corrupted.\n"
    "## 13) Top structure canary is corrupted.\n"
    "## 14) Data is corrupted (main buffer).\n"
    "## 15) Stack is corrupted.\n"
    "## 16) \n");
    

    fprintf(log, "\n################################################################\n\n\n\n");
    
    fclose(log);
    
    return NO_ERRORS;
}
