#include "StackDB.h"
#include "StackStruct.h"

int StackError(Stack_t* stk)
{
    int errFlag = 0;
    
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
