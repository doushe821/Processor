#ifndef COMMANFS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include <stdlib.h>
#include <inttypes.h>


const size_t ARGLEN_MAX = 64;
const size_t COMMANDNAME_MAX = 32; 
const int NUMBER_OF_COMMANDS = 32;

enum commands_codes
{
    CMD_UNKNOWN = 0x0000,
    CMD_PUSH    = 0x0001,
    CMD_ADD     = 0x0003, 
    CMD_SUB     = 0x0004,
    CMD_DIV     = 0x0005,
    CMD_MUL     = 0x0006,
    CMD_POW     = 0x0007,
    CMD_SQRT    = 0x0008, 
    CMD_SIN     = 0x0009,
    CMD_POP     = 0x000B,
    CMD_OUT     = 0x0010,

    CMD_IN      = 0x0011,

    CMD_JMP     = 0x0014, 
    CMD_JMB     = 0x0015,
    CMD_JMA     = 0x0016,
    CMD_JME     = 0x0017,
    CMD_JMN     = 0x0018,
    CMD_JMBE    = 0x0019,
    CMD_JMAE    = 0x001A,
    CMD_CALL    = 0x001B,
    CMD_RET     = 0x001C,
     
    CMD_SLEEP   = 0x001D,
    CMD_DRAW    = 0x001E,
    CMD_HLT     = 0x001F,
};

struct SPU;
struct Assembly_t;

int HandlePush  (SPU* Baikal);
int HandleAdd   (SPU* Baikal);
int HandleSub   (SPU* Baikal);
int HandleDiv   (SPU* Baikal);
int HandleMul   (SPU* Baikal);
int HandlePow   (SPU* Baikal);
int HandleSqrt  (SPU* Baikal);
int HandleSin   (SPU* Baikal);
int HandleOut   (SPU* Baikal);
int HandlePop   (SPU* Baikal);
int HandleJmp   (SPU* Baikal);
int HandleJmb   (SPU* Baikal);
int HandleJma   (SPU* Baikal);
int HandleJme   (SPU* Baikal);
int HandleJmn   (SPU* Baikal);
int HandleJmbe  (SPU* Baikal);
int HandleJmae  (SPU* Baikal);
int HandleCall  (SPU* Baikal);
int HandleRet   (SPU* Baikal);
int HandleSleep (SPU* Baikal);
int HandleDraw  (SPU* Baikal);
int HandleIn    (SPU* Baikal);
int HandleHlt   (SPU* Baikal);
int HandleUnknown(SPU* Baikal);


int ParseJump (Assembly_t* Asm, char* buffer);
int ParsePush (Assembly_t* Asm, char* buffer);
int ParsePop  (Assembly_t* Asm, char* buffer);
int ParseCall (Assembly_t* Asm, char* buffer);
int ParseSleep(Assembly_t* Asm, char* buffer);
int ParseDraw (Assembly_t* Asm, char* buffer); 
int ParseNoArg(Assembly_t* Asm, char* buffer);
int ParseUnknown(Assembly_t* Asm, char* buffer);


struct Command_t
{
    char Name[COMMANDNAME_MAX];
    uint8_t OpCode;
    int (*ParseFunc)(Assembly_t*, char*);
    int (*HandleFunc)(SPU*);
    int HasArg;
};

const Command_t CommandsStruct[NUMBER_OF_COMMANDS] = // TODO CODE GEN
{
    {"NULL" ,  0,  ParseUnknown, HandleUnknown, 0},
    {"push" ,  1,  ParsePush   , HandlePush   , 1},
    {"NULL" ,  2,  ParseUnknown, HandleUnknown, 0},
    {"add"  ,  3,  ParseNoArg  , HandleAdd    , 0},
    {"sub"  ,  4,  ParseNoArg  , HandleSub    , 0},
    {"div"  ,  5,  ParseNoArg  , HandleDiv    , 0},
    {"mul"  ,  6,  ParseNoArg  , HandleMul    , 0},
    {"pow"  ,  7,  ParseNoArg  , HandlePow    , 0},
    {"sqrt" ,  8,  ParseNoArg  , HandleSqrt   , 0},
    {"sin"  ,  9,  ParseNoArg  , HandleSin    , 0},
    {"NULL" ,  10, ParseUnknown, HandleUnknown, 0},
    {"pop"  ,  11, ParsePop    , HandlePop    , 1},
    {"NULL" ,  12, ParseUnknown, HandleUnknown, 0},
    {"NULL" ,  13, ParseUnknown, HandleUnknown, 0},
    {"NULL" ,  14, ParseUnknown, HandleUnknown, 0},
    {"NULL" ,  15, ParseUnknown, HandleUnknown, 0},
    {"out"  ,  16, ParseNoArg  , HandleOut    , 0},
    {"in"   ,  17, ParseNoArg  , HandleIn     , 0},
    {"NULL" ,  18, ParseUnknown, HandleUnknown, 0},
    {"NULL" ,  19, ParseUnknown, HandleUnknown, 0},
    {"jmp"  ,  20, ParseJump   , HandleJmp    , 1},
    {"jmb"  ,  21, ParseJump   , HandleJmb    , 1},
    {"jma"  ,  22, ParseJump   , HandleJma    , 1},
    {"jme"  ,  23, ParseJump   , HandleJme    , 1},
    {"jmn"  ,  24, ParseJump   , HandleJmn    , 1},
    {"jmbe" ,  25, ParseJump   , HandleJmbe   , 1},
    {"jmae" ,  26, ParseJump   , HandleJmae   , 1},
    {"call" ,  27, ParseCall   , HandleCall   , 1},
    {"ret"  ,  28, ParseNoArg  , HandleRet    , 0},
    {"sleep",  29, ParseSleep  , HandleSleep  , 1},
    {"draw" ,  30, ParseDraw   , HandleDraw   , 1},
    {"hlt"  ,  31, ParseNoArg  , HandleHlt    , 0},
};


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