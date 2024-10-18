#ifndef ASSEMBLER_INCLUDED
#define ASSEMBLER_INCLUDED

#include <inttypes.h>
const uint32_t version = 1;
const uint32_t sign = 44001;

#ifndef NDEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG() 
#endif

#endif
