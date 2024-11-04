
#ifndef STACK_DB
#define STACK_DB


//#define NDEBUG

#include "Stack.h"
#ifdef NDEBUG
#define ON_DEBUG(code) 
#else
#define ON_DEBUG(code) code
#endif //NDEBUG



const size_t UNLIKELY_STACK_SIZE = 666666666666666666; // Very unlikely to appear, used as a sign of stack underflow.
const int StructCanaryGuardBotREF = 78;
const int StructCanaryGuardTopREF = 78;
const int DataCanaryGuardBotREF = 78;
const int DataCanaryGuardTopREF = 78;

int StackError(Stack_t* stk);

#ifndef NDEBUG

#define $ fprintf(stderr, "%s:%d\n", __FILE__, __LINE__) // BIG MONEY
#define STACK_INIT(Stack, inCap) strcat(*Stack.stkName, #Stack); StackInit((Stack), inCap)

#else

#define STACK_ASSERT(Stack)

#endif //NDEBUG

#endif



