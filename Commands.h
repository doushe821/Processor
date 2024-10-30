#ifndef COMMANFS_INCLUDED
#define COMMANDS_INCLUDED

#include <stdlib.h>
#include <inttypes.h>

enum commands
{
    CMD_PUSH  = 0x0001,
    CMD_ADD   = 0x0003, 
    CMD_SUB   = 0x0004,
    CMD_DIV   = 0x0005,
    CMD_MUL   = 0x0006,
    CMD_POW   = 0x0007,
    CMD_SQRT  = 0x0008, 
    CMD_SIN   = 0x0009,

    CMD_POP   = 0x000B,
    CMD_OUT   = 0x0010,

    CMD_IN    = 0x0011,

    CMD_JMP   = 0x0014, 
    CMD_JMB   = 0x0015,
    CMD_JMA   = 0x0016,
    CMD_JME   = 0x0017,
    CMD_JMN   = 0x0018,
    CMD_JMBE  = 0x0019,
    CMD_JMAE  = 0x001A,
    CMD_CALL  = 0x001B,
    CMD_RET   = 0x001C,
     
    CMD_SLEEP = 0x001D,
    CMD_DRAW  = 0x001E,
    CMD_HLT   = 0x001F,
};

const char CommandNames[NUMBER_OF_COMMANDS][COMMANDNAME_MAX] = 
{
    "null", "push", "null", "add", "sub", "div", "mul", "pow",
    "sqrt", "sin", "null", "pop", "null", "null", "null", "null", "out", "in", 
    "null", "null", "jmp", "jmb", "jma","jme", "jmn", "jmbe", "jmae", "call", "ret",
    "sleep", "draw", "hlt"
}; // massive of structures

const uint8_t RAM_REG_CONSTVAL = 7;
const uint8_t RAM_REG          = 6;
const uint8_t RAM_CONSTVAL     = 5;
const uint8_t REG_CONSTVAL     = 3;
const uint8_t REG              = 2; 
const uint8_t CONSTVAL         = 1;

struct Header
{
    uint32_t sign;
    uint32_t ver;
    uint32_t size;
    uint32_t reserverd;
};

const size_t SIZE_OF_HEADER = 4;
const size_t NUMBER_OF_REGS = 10;
const size_t REGNAME_MAX = 2;
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