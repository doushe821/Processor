#include "Stack.h"
#include "StackDB.h"
#include "StackStruct.h"

static int POISON = -666;
static int* POISON_PTR = &POISON; 

int StackInit(Stack_t** stk, size_t InitCapacity, size_t ElemSize)
{
    
    if(*stk != NULL)
    {
        if((*stk)->initialised)
        {
            return 0;
        }
    }

    *stk = (Stack_t*)calloc(1, sizeof(Stack_t));
    (*stk)->elSize = ElemSize;

    if(InitCapacity < MinCapacity)  
    {
        if(((*stk)->data = calloc(MinCapacity, ElemSize)) == NULL)
        {
            fprintf(stderr, "Dynamic Memory dead\n");
            (*stk)->Error += ALLOC_ERROR;
            return ALLOC_ERROR; 
        }
        (*stk)->capacity = MinCapacity;
    }    
    else
    {
        if(((*stk)->data = calloc(InitCapacity ON_DEBUG( + 2), ElemSize)) == NULL)
        {
            fprintf(stderr, "Dynamic Memory dead\n");
            (*stk)->Error += ALLOC_ERROR;
            return ALLOC_ERROR; 
        }
        (*stk)->capacity = InitCapacity;
    }

    (*stk)->elSize = ElemSize;
    (*stk)->size = 0;
    (*stk)->initialised = 1;
    
    return NO_ERRORS;
}
                   
int StackDtor(Stack_t* stk)
{
    
    void* ptr = memset(stk->data, 0, stk->elSize*stk->capacity);
    
    if(ptr == NULL)
    {
        stk->Error += MEMSET_FAILURE;
        free(stk->data);
        return MEMSET_FAILURE;
    }
    stk->initialised = 0;
    free(stk->data);
    free(stk);

    return NO_ERRORS; 
}

int StackPush(Stack_t* stk, void* elem)
{
     

    if(stk->size >= stk->capacity)
    {
        if(StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }
    
    memcpy((char*)stk->data + (stk->size ON_DEBUG( + 1))*stk->elSize, elem, stk->elSize);
    stk->size++;
    
    #ifndef NDEBUG
    stk->DataHash = hash(stk->data, stk->capacity + 2);
    stk->StructHash = hash(stk, sizeof(Stack_t));
    #endif //NDEBUG

    

    return NO_ERRORS;
}

int VStackInit(Stack_t** stk, size_t InitCapacity)
{
    
    if(*stk != NULL)
    {
        if((*stk)->initialised)
        {
            return 0;
        }
    }

    *stk = (Stack_t*)calloc(1, sizeof(Stack_t));

    if(InitCapacity < MinCapacity)  
    {
        if(((*stk)->data = calloc(MinCapacity, sizeof(char))) == NULL)
        {
            fprintf(stderr, "Dynamic Memory dead\n");
            (*stk)->Error += ALLOC_ERROR;
            return ALLOC_ERROR; 
        }
        (*stk)->capacity = MinCapacity;
    }    
    else
    {
        if(((*stk)->data = calloc(InitCapacity, sizeof(char))) == NULL)
        {
            fprintf(stderr, "Dynamic Memory dead\n");
            (*stk)->Error += ALLOC_ERROR;
            return ALLOC_ERROR; 
        }
        (*stk)->capacity = InitCapacity;
    }

    (*stk)->elSize = sizeof(char);
    (*stk)->size = 0;
    (*stk)->initialised = 1;
    
    return NO_ERRORS;
}

int VStackPush(Stack_t* stk, const void* elem, size_t size)
{
    if(stk->size >= stk->capacity / 1.5)
    {
        if(StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }
    memcpy((char*)stk->data + stk->size, elem, size);
    stk->size += size;
    return NO_ERRORS;
}

int StackLook(Stack_t* stk, size_t size)
{
    fprintf(stderr, "//\n");
    for(size_t i = 0; i < size; i++)
    {
        fprintf(stderr, "%d", *((char*)stk->data + stk->size - i - 1));
    }
    fprintf(stderr, "\n//\n");
    return NO_ERRORS;
}

size_t GetStackSize(Stack_t* stk)
{
    return stk->size;
}

void* GetDataPointer(Stack_t* stk)
{
    return stk->data;
}

int StackPop(Stack_t* stk, void* elem)
{
    
    

    if(stk->size <= 0)
    {
        stk->Error = STACK_UNDERFLOW;
        fprintf(stderr, "Tried to pop 0 size stack\n");
        return STACK_UNDERFLOW;
    }
    
    if(stk->size <= stk->capacity/4)
    {
        
        if(StackResize(stk, true) != 0)
        {
            stk->Error = ALLOC_ERROR;
            return ALLOC_ERROR;
        }
        
    }

    memcpy(elem, (char*)stk->data + stk->size*stk->elSize, stk->elSize);
    
    memcpy((char*)stk->data + stk->size*stk->elSize, POISON_PTR, stk->elSize);
    
    stk->size--;


    #ifndef NDEBUG
    stk->DataHash = hash(stk->data, stk->capacity + 2);
    stk->StructHash = hash(stk, sizeof(Stack_t));
    #endif //NDEBUG
    
    return NO_ERRORS;
}

int FStackPop(Stack_t* stk)
{

    if(stk->size <= 0)
    {
        stk->Error = STACK_UNDERFLOW;
        fprintf(stderr, "Tried to pop 0 size stack\n");
        return STACK_UNDERFLOW;
    }
    
    if(stk->size <= stk->capacity/4)
    {
        
        if(StackResize(stk, true) != 0)
        {
            stk->Error = ALLOC_ERROR;
            return ALLOC_ERROR;
        }
        
    }
    
    memcpy((char*)stk->data + stk->size*stk->elSize, POISON_PTR, stk->elSize);
    
    stk->size--;


    #ifndef NDEBUG
    stk->DataHash = hash(stk->data, stk->capacity + 2);
    stk->StructHash = hash(stk, sizeof(Stack_t));
    #endif //NDEBUG

    
    
    return NO_ERRORS;
}

int StackResize(Stack_t* stk, bool downSizeFlag)
{

    //

    if(downSizeFlag)
    {
        if((stk->data = wrecalloc(stk->data, stk->capacity/REALLOC_COEF ON_DEBUG( + 2), stk->elSize, stk->size)) == 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data); // TODO 
            
            return ALLOC_ERROR;
        }

        stk->capacity /= REALLOC_COEF;

        return NO_ERRORS;
    }

    else
    {
        if((stk->data = wrecalloc(stk->data, stk->capacity*REALLOC_COEF ON_DEBUG( + 2), stk->elSize, stk->size))== 0)
        {
            stk->Error += ALLOC_ERROR;
            return ALLOC_ERROR;
        }

        stk->capacity *= REALLOC_COEF;

        return NO_ERRORS;
    } 

    return NO_ERRORS;
}

void* wrecalloc(void* ptr, size_t num, size_t size, size_t PrevSize) //POISON
{
    
    if(num == 0)
    {
        void* tptr = calloc(512, size);
        if(tptr == NULL)
        {
            free(tptr);
            return NULL;
        }
        return tptr;
    }

    void* tptr = calloc(num, size);

    if(tptr == NULL)
    {
        free(tptr);
        return NULL;
    }
    
    memset(ON_DEBUG((char*))tptr ON_DEBUG( + 1*size) , POISON, (num - 1)*size);
    memcpy((char*)tptr, ptr, (PrevSize)*size);
    
    free(ptr);
    
    return tptr;
} 

int ErrParse(Stack_t* stk)
{
    switch(stk->Error)
    {
        case UNDEFINED_ERROR:
        {
            fprintf(stderr, "Undefined error, make a report, please.\n");
            return UNDEFINED_ERROR;
        }

        case ALLOC_ERROR:
        {
            fprintf(stderr, "Cannot allocate memory.\n");
            return ALLOC_ERROR;
        }

        case FILE_CREATION_ERROR:
        {
            fprintf(stderr, "Could not open existing or create new log.txt file.\n");
            return FILE_CREATION_ERROR;
        }

        case MEMSET_FAILURE:
        {
            fprintf(stderr, "Could not initialise allocated memory.\n");
            return MEMSET_FAILURE;
        }

        case STACK_UNDERFLOW:
        {
            fprintf(stderr, "You tried to grab an element from an empty stack.\n");
            return STACK_UNDERFLOW;
        }

        case STACK_OVERFLOW:
        {
            fprintf(stderr, "Stack overflow.\n");
            return STACK_OVERFLOW;
        }

        default:
        {
            fprintf(stderr, "An undefined error appeared. Please, make a report to make program better.\n");
            return UNDEFINED_ERROR;
        }
    }
}



