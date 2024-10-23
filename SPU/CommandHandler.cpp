#include "Processor.h"

static int HandlePush(SPU* Baikal);
static int HandlePop(SPU* Baikal);

int doCommand(SPU* Baikal)
{
    switch(Baikal->cmdSheet.buf[Baikal->ip])
    {
        case PUSH:
        {
            $;
            Baikal->ip++;
            HandlePush(Baikal);
            return 0;
        }
        case POP:
        {
            Baikal->ip++;
            HandlePop(Baikal);
            return 0;
        }
        case ADD:
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            double c = a + b; // a += b for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case SUB:
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            double c = b - a; // b -= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case DIV:
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            double c = b / a; // b /= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case MUL:
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a); // Should check?
            StackPop(Baikal->stk, &b); // Should check?
            double c = b * a; // b *= a for optimisation?
            StackPush(Baikal->stk, &c); // Should check?
            Baikal->ip++;
            return 0;
        }
        case POW:
        {
            double a = 0;
            double b = 0;
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
            double a = 0;
            StackPop(Baikal->stk, &a);
            printf("%lg\n", a);
            Baikal->ip += 1;
            return 0;
        }
        case JMP:
        {
            Baikal->ip += 1;
            uint32_t ip = 0;
            memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
            Baikal->ip = ip;
            return 0;
        }
        case JMB: 
        {
            Baikal->ip += 1;
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) < 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case JMA: 
        {
            Baikal->ip += 1;
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) > 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case JME: 
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) == 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case JMN: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) != 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case JMBE: 
        {
            double a = 0;
            double b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) <= 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case JMAE: 
        {
            int a = 0;
            int b = 0;
            StackPop(Baikal->stk, &a);
            StackPop(Baikal->stk, &b);
            if(doublecmp(b, a, EPS) >= 0)
            {
                uint32_t ip = 0;
                memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
                Baikal->ip = ip;
                return 0;
            }
            Baikal->ip += sizeof(uint32_t);
            return 0;
        }
        case SLEEP:
        {
            Baikal->ip += 1;
            uint32_t slpTime = 0;
            memcpy(&slpTime, Baikal->cmdSheet.buf + Baikal->ip, sizeof(slpTime));
            usleep(slpTime);
            Baikal->ip += sizeof(slpTime);

            return 0;
        }
        case DRAW:
        {
            fprintf(stdout, "\033[1J");
            uint32_t width = Baikal->cmdSheet.buf[Baikal->ip + 1];
            uint32_t height = Baikal->cmdSheet.buf[Baikal->ip + 2];
            Baikal->ip += 3;

            for(uint32_t i = 0; i < height; i++)
            {
                for(uint32_t j = 0; j < width; j++)
                {
                    fprintf(stdout, "%c", ' ' + ('#' - ' ')*Baikal->ram[i*width + j]); //177 32 for space

                }
                fprintf(stdout, "\n");
            }
            return 0;
        }
        case HLT:
        {
            return HLT;
        }
    }
    return UNDEFINED_ERROR; // TODO
}




static int HandlePush(SPU* Baikal)
{
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP + 1 = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));
    union res
    {
        double tRes;
        uint32_t addr;
    };

    uint8_t ArgCode = 0;
    memcpy(&ArgCode, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ArgCode));
    Baikal->ip += sizeof(ArgCode);
    double result = 0;
    bool isAddr = false;

    if(ArgCode & REG_MASK)
    {
        
        ON_DEBUG(fprintf(stderr, "## IS REG\n"));
        uint32_t regID = 0;
        memcpy(&regID, Baikal->cmdSheet.buf + Baikal->ip, sizeof(regID));
        result = Baikal->Reg[regID];
        Baikal->ip += sizeof(regID);
        isAddr = true;
    }

    if(ArgCode & C_MASK)
    {
        $;
        ON_DEBUG(fprintf(stderr, "  ## IS C\n"));
        res constVal = {0};
        if(isAddr)
        {
            memcpy(&constVal.addr, Baikal->cmdSheet.buf + Baikal->ip, sizeof(constVal.addr));
            result += constVal.addr;
            Baikal->ip += sizeof(constVal.addr); 
        }
        else
        {
            memcpy(&constVal.tRes, Baikal->cmdSheet.buf + Baikal->ip, sizeof(constVal.tRes));
            result += constVal.tRes;
            Baikal->ip += sizeof(constVal.tRes);
        }

    }

    if(ArgCode & RAM_MASK)
    {
        ON_DEBUG(fprintf(stderr, "## IS RAM\n"));
        uint32_t addr = result;
        memcpy(&result, Baikal->ram + addr, sizeof(double));
        result = Baikal->ram[addr];
    }
    $;
    StackPush(Baikal->stk, &result);
    $;
    ON_DEBUG(fprintf(stderr, "## IP AFTER PUSH = %zu\n", Baikal->ip));
    return 0; //TODO err codes
}




static int HandlePop(SPU* Baikal)
{

    uint8_t ArgCode = Baikal->cmdSheet.buf[Baikal->ip];
    
    (Baikal->ip) += 1;

    void* addr = 0;
    uint32_t tRes = 0;

    if(ArgCode & REG_MASK)
    {
        memcpy(&tRes, Baikal->cmdSheet.buf + Baikal->ip, sizeof(tRes));
        addr = &Baikal->Reg[tRes];
        Baikal->ip += sizeof(tRes);
    }

    if(ArgCode & RAM_MASK)
    {
        if(ArgCode & REG_MASK && ArgCode & C_MASK)
        {
            uint32_t add = 0;
            memcpy(&add, Baikal->cmdSheet.buf + Baikal->ip, sizeof(add));
            tRes += add;
            addr = &Baikal->ram[tRes];
            Baikal->ip += sizeof(add);
        }

        else if(ArgCode & C_MASK)
        {
            memcpy(&tRes, Baikal->cmdSheet.buf + Baikal->ip, sizeof(tRes));
            addr = &Baikal->ram[tRes];
            Baikal->ip += sizeof(tRes);
        }

        else
        {
            addr = &Baikal->ram[tRes];   
        }
    }

    StackPop(Baikal->stk, addr);

    return 0; //TODO err codes
}