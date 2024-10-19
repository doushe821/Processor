#include "Processor.h"

static int HandlePush(SPU* Baikal);
static int HandlePop(SPU* Baikal);

int doCommand(SPU* Baikal)
{
    switch(Baikal->cmdSheet.buf[Baikal->ip])
    {
        case PUSH:
        {
            HandlePush(Baikal);
            return 0;
        }
        case POP:
        {
            HandlePop(Baikal);
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
        case OUT:
        {
            int a = 0;
            StackPop(Baikal->stk, &a);
            printf("%d\n", a);
            Baikal->ip += 1;
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
        case SLEEP:
        {
            usleep(Baikal->cmdSheet.buf[Baikal->ip + 1]);
            Baikal->ip += 2;
            return 0;
        }
        case DRAW:
        {
            fprintf(stderr, "\033[1J");
            uint32_t width = Baikal->cmdSheet.buf[Baikal->ip + 1];
            uint32_t height = Baikal->cmdSheet.buf[Baikal->ip + 2];
            Baikal->ip += 3;

            for(uint32_t i = 0; i < height; i++)
            {
                for(uint32_t j = 0; j < width; j++)
                {
                    fprintf(stderr, "%c", ' ' + ('#' - ' ')*Baikal->ram[i*width + j]); //177 32 for space

                }
                fprintf(stderr, "\n");
            }
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




static int HandlePush(SPU* Baikal)
{
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));
    (Baikal->ip)++;
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP + 1 = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));
    uint32_t ArgCode = Baikal->cmdSheet.buf[Baikal->ip];
    uint32_t result = 0;

    if(ArgCode & REG_MASK)
    {
        //ON_DEBUG(fprintf(stderr, "## IS REG\n"));
        (Baikal->ip)++;
        result = Baikal->Reg[Baikal->cmdSheet.buf[Baikal->ip]];
    }

    if(ArgCode & C_MASK)
    {
        //ON_DEBUG(fprintf(stderr, "## IS C\n"));
        (Baikal->ip)++;
        result += Baikal->cmdSheet.buf[Baikal->ip];
    }

    if(ArgCode & RAM_MASK)
    {
        //ON_DEBUG(fprintf(stderr, "## IS RAM\n"));
        uint32_t addr = result;
        result = Baikal->ram[addr];
    }

    StackPush(Baikal->stk, &result);

    (Baikal->ip)++;

    return 0; //TODO err codes
}




static int HandlePop(SPU* Baikal)
{
    (Baikal->ip)++;
    uint32_t ArgCode = Baikal->cmdSheet.buf[Baikal->ip];

    int* addr = 0;
    int tres = 0;

    if(ArgCode & REG_MASK)
    {
        (Baikal->ip)++;
        tres = Baikal->cmdSheet.buf[Baikal->ip];
        addr = &Baikal->Reg[Baikal->cmdSheet.buf[Baikal->ip]];
    }

    if(ArgCode & RAM_MASK)
    {
        if(ArgCode & REG_MASK && ArgCode & C_MASK)
        {
            (Baikal->ip)++;
            tres += Baikal->cmdSheet.buf[Baikal->ip];
            addr = &Baikal->ram[tres];
        }

        else if(ArgCode & C_MASK)
        {
            (Baikal->ip)++;
            tres = Baikal->cmdSheet.buf[Baikal->ip];
            addr = &Baikal->ram[tres];
        }

        else
        {
            (Baikal->ip)++;
            addr = &Baikal->ram[tres];   
        }
    }

    StackPop(Baikal->stk, addr);

    (Baikal->ip)++;

    return 0; //TODO err codes
}