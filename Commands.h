#ifndef COMMANFS_INCLUDED
#define COMMANDS_INCLUDED

#include <stdlib.h>
#include <inttypes.h>

enum commands
{
    PUSH = 0x0001,
    ADD = 0x0003, 
    SUB = 0x0004,
    DIV = 0x0005,
    MUL = 0x0006,
    POW = 0x0007,
    SQRT = 0x0008, 
    SIN = 0x0009,
    PUSHR = 0x000A,
    POP = 0x000B,
    OUT = 0x0010,

    JMP = 0x0014, 
    JMB = 0x0015,
    JMA = 0x0016,
    JME = 0x0017,
    JMN = 0x0018,
    JMBE = 0x0019,
    JMAE = 0x001A,
    
    HLT = 0x007F,
    MEOW = 0x029A, 
};


struct Header
{
    uint32_t sign;
    uint32_t ver;
    uint32_t size;
    uint32_t reserverd;
};

const size_t sizeofHeader = 4;

const char REG_NAMES[10][3] = 
{
    "NX", "AX", "BX", "CX", "DX", "EX", "FX", "GX", "HX", "IX",
};

enum Registers
{
    NX = 0,
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4,
    EX = 5,
    FX = 6,
    GX = 7,
    HX = 8,
    IX = 9,
};
#endif