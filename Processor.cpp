#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "Stack.h"
#include "FileManager.h"
#include "Commands.h"

#include "BigMoney.h"
#include "Miscellaneous.h"


enum ErrCodes
{
    FOPEN_ERROR = 1,
    ASSEMBLER_ERROR,
    READING_ERROR,
    BUFFERISATION_ERROR,
    COMPILER_ERROR,
    STACK_INIT_ERROR,
    PUSH_ERROR,
    POP_ERROR,
    HEADERINIT_ERROR,
};



struct Buffer
{
    int* buf;
    int err;
    size_t size;
};

struct SPU
{
    Buffer cmdSheet;
    Stack_t* stk;
    int Reg[10];
    size_t ip;
    int ram[131072];
};


struct Files
{
    FILE* obj;
    bool err;
};

struct Files CmdOpenFile(int c, char** v);
struct Buffer Bufferise(FILE* obj, const Header head);
int Process(Buffer Sheet);
int doCommand(SPU* spu);
int HeaderInit(Header* head, FILE* obj);

int main(int argc, char* argv[])
{
    Files files = {};
    files = CmdOpenFile(argc, argv);
    if(files.err)
    {
        return FOPEN_ERROR;
    }

    Header head = {};

    if(HeaderInit(&head, files.obj) != 0)
    {
        return HEADERINIT_ERROR;
    }

    Buffer cmdSheet = Bufferise(files.obj, head);
    if(cmdSheet.buf == NULL)
    {      
        return BUFFERISATION_ERROR;
    }
    fclose(files.obj);

    if(Process(cmdSheet) != 0)
    {
        return COMPILER_ERROR;
    }

    free(cmdSheet.buf);
    return 0;
}

int HeaderInit(Header* head, FILE* obj)
{
    if(fread(&head->sign, 1, sizeof(uint32_t), obj) == 0)
    {
        return READING_ERROR;
    }
    if(fread(&head->ver, 1, sizeof(uint32_t), obj) == 0)
    {
        return READING_ERROR;
    }
    if(fread(&head->size, 1, sizeof(uint32_t), obj) == 0)
    {
        return READING_ERROR;
    }
    if(fread(&head->reserverd, 1, sizeof(uint32_t), obj) == 0)
    {
        return READING_ERROR;
    }
    
    return 0;
}

int Process(Buffer buf)
{
    SPU Baikal = {}; 
    
    if(StackInit(&Baikal.stk, 512, sizeof(*Baikal.cmdSheet.buf)) != 0)
    {
        return STACK_INIT_ERROR;
    }
    Baikal.cmdSheet = buf; 
    
    while(true)
    {
        ON_DEBUG(fprintf(stderr, "## OPERATION CODE = %d\n", Baikal.cmdSheet.buf[Baikal.ip]));
        if(doCommand(&Baikal) == HLT)
        {
            break;
        }
        ON_DEBUG(fprintf(stderr, "IP = %zu\n", Baikal.ip));
        usleep(500000);
    }
    StackDtor(Baikal.stk);
    return 0;
}

int doCommand(SPU* Baikal)
{
    switch(Baikal->cmdSheet.buf[Baikal->ip])
    {
        case PUSH:
        {
            int pushval = Baikal->cmdSheet.buf[Baikal->ip + 1];
            if(StackPush(Baikal->stk, &pushval) != 0)
            {
                return PUSH_ERROR;
            }
            Baikal->ip += 2;
            return 0;
        }
        case POP:
        {
            switch(Baikal->cmdSheet.buf[Baikal->ip + 1])
            {
                case NX:
                {
                    if(FStackPop(Baikal->stk) != 0)
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case AX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[AX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case BX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[BX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case CX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[CX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case DX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[DX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case EX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[EX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case FX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[FX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case GX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[GX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case HX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[HX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
                case IX:
                {
                    if(StackPop(Baikal->stk, &Baikal->Reg[IX]))
                    {
                        return POP_ERROR;
                    }
                    Baikal->ip += 2;
                    return 0;
                }
            }
            return 0;
        }
        case ADD:
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            int c = a + b; // a += b for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case SUB:
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            int c = b - a; // b -= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case DIV:
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            int c = b / a; // b /= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case MUL:
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            int c = b * a; // b *= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case POW:
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            for(int i = 0; i < a; i++)
            {
                b *= b;
            }
            StackPush(Baikal->stk, &b); // Should check?
            Baikal->ip++;
            return 0;
        }
        case SQRT:
        {
            Baikal->ip += 1;
            return -77; // UNDER DEVELOPMENT
        }
        case SIN:
        {
            Baikal->ip += 1;
            return -169; // UNDER DEVELOPMENT
        }
        case MEOW:
        {
            fprintf(stdout, "%s", MEOW666);
            Baikal->ip = 666;
            return 666;
        }
        case OUT:
        {
            int a = 0;
            StackPop(Baikal->stk, &a);
            printf("%d\n", a);
            Baikal->ip += 1;
            return 0;
        }
        case PUSHR:
        {
            int a = 0;
            if(StackPop(Baikal->stk, &a) != 0)
            {
                Baikal->cmdSheet.err = POP_ERROR;
            }
            Baikal->Reg[Baikal->ip + 1] = a;
            Baikal->ip += 2;
            return 0;
        }
        case JMP:
        {
            Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
            return 0;
        }
        case JMB: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b < a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case JMA: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b > a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case JME: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b == a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case JMN: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b != a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case JMBE: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b <= a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case JMAE: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(b >= a)
            {
                Baikal->ip = Baikal->cmdSheet.buf[Baikal->ip + 1];
                return 0;
            }
            Baikal->ip += 2;
            return 0;
        }
        case HLT:
        {
            return HLT;
        }
    }
    Baikal->ip++;
    return UNDEFINED_ERROR; // TODO
}

struct Buffer Bufferise(FILE* obj, const Header head)
{
    struct Buffer Bout = {};

    Bout.buf = (int*)calloc(head.size, sizeof(int));
    Bout.size = head.size;
    if(fread(Bout.buf, Bout.size, sizeof(*Bout.buf), obj) == 0)
    {
        Bout.err = READING_ERROR;
    }

    return Bout;
}

struct Files CmdOpenFile(int c, char** v)
{
    FILE* object = NULL;
    struct Files file = {};
    if(c > 1)
    {
        object = fopen(v[1], "r+b");
        if(object == NULL)
        {
            file.err = true;
            return file;
        }
        file.obj = object;
    }
    else
    {
        object = fopen("object", "r+b");
        if(object == NULL)
        {
            file.err = true;
            return file;
        }
        file.obj = object;
    }

    return file;
    
}
