#include "Processor.h"

static int HandlePush(SPU* Baikal);
static int HandlePop (SPU* Baikal);
static int HandleRet (SPU* Baikal);
static int HandleCall(SPU* Baikal);

int doCommand(SPU* Baikal)
{
    switch(Baikal->cmdSheet.buf[Baikal->ip])
    {
        case CMD_PUSH:
        {
            
            Baikal->ip++;
            HandlePush(Baikal);
            return 0;
        }
        case CMD_POP:
        {
            Baikal->ip++;
            HandlePop(Baikal);
            return 0;
        }
        case CMD_ADD:
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
        case CMD_SUB:
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
        case CMD_DIV:
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
        case CMD_MUL:
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
        case CMD_POW:
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
        case CMD_SQRT:
        {
            Baikal->ip += 1;
            double a = 0;
            StackPop(Baikal->stk, &a);
            double b = sqrt(a);
            StackPush(Baikal->stk, &b);
            return 0; // UNDER DEVELOPMENT
        }
        case CMD_SIN:
        {
            Baikal->ip += 1;
            return -169; // UNDER DEVELOPMENT
        }
        case CMD_OUT:
        {
            double a = 0;
            StackPop(Baikal->stk, &a);
            printf("%.6lf\n", a);
            Baikal->ip += 1;
            return 0;
        }
        case CMD_JMP:
        {
            Baikal->ip += 1;
            uint32_t ip = 0;
            memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
            Baikal->ip = ip;
            return 0;
        }
        case CMD_JMB: 
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
        case CMD_JMA: 
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
        case CMD_JME: // TODO prefixes
        {
            Baikal->ip += 1;
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
        case CMD_JMN: 
        {
            Baikal->ip += 1;
            double a = 0;
            double b = 0;
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
        case CMD_JMBE: 
        {
            Baikal->ip += 1;
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
        case CMD_JMAE: 
        {
            Baikal->ip += 1;
            double a = 0;
            double b = 0;
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
        case CMD_CALL:
        {
            Baikal->ip += 1;
            HandleCall(Baikal);
            return 0;
        }
        case CMD_RET:
        {
            HandleRet(Baikal);
            return 0;
        }
        case CMD_SLEEP:
        {
            Baikal->ip += 1;
            uint32_t slpTime = 0;
            memcpy(&slpTime, Baikal->cmdSheet.buf + Baikal->ip, sizeof(slpTime));
            usleep(slpTime);
            Baikal->ip += sizeof(slpTime);
            

            return 0;
        }
        case CMD_DRAW:
        {
            Baikal->ip += 1;
            fprintf(stdout, "\033[1J");
            uint32_t width  = 0;
            uint32_t height = 0;

            memcpy(&width, Baikal->cmdSheet.buf + Baikal->ip, sizeof(width));

            Baikal->ip += sizeof(width);
            memcpy(&height, Baikal->cmdSheet.buf + Baikal->ip, sizeof(height));
            Baikal->ip += sizeof(height);


            for(uint32_t i = 0; i < height; i++)
            {
                for(uint32_t j = 0; j < width; j++)
                {
                    double out = 0;
                    char pixel = 0;
                    memcpy(&out, Baikal->ram + sizeof(out)*(i*width + j), sizeof(out));                  
                    if(doublecmp(out, 0, EPS) != 0)
                    {
                        pixel = 1;
                    }
                    fprintf(stdout, "%c", ' ' + ('#' - ' ')*pixel);
                }
                fprintf(stdout, "\n");
            }
            return 0;
        }
        case CMD_IN:
        {
            Baikal->ip += 1;
            double a = 0;
            if(scanf("%lf", &a) == 0)
            {
                return READING_ERROR;
            }
            StackPush(Baikal->stk, &a);
            return 0;
        }
        case CMD_HLT:
        {
            return CMD_HLT;
        }
    }
    return UNDEFINED_ERROR; // TODO
}



static int HandleCall(SPU* Baikal)
{
    uint32_t ip = 0;
    memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
    Baikal->ip += sizeof(ip);

    StackPush(Baikal->CallStk, &Baikal->ip);

    Baikal->ip = ip;
    return 0;
}

static int HandleRet(SPU* Baikal)
{
    uint32_t retIP = 0;
    StackPop(Baikal->CallStk, &retIP);
    Baikal->ip = retIP;
    return 0;
}

static int HandlePush(SPU* Baikal)
{
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));
    //ON_DEBUG(fprintf(stderr, "## VAL FOR IP + 1 = %d\n", Baikal->cmdSheet.buf[Baikal->ip]));

    uint8_t ArgCode = 0;
    memcpy(&ArgCode, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ArgCode));
    Baikal->ip += sizeof(ArgCode);
    uint32_t addr = 0;
    double result = 0;
    bool isAddr = false;

    if(ArgCode & RAM_MASK)
    {
        isAddr = true;
    }
    if(ArgCode & REG_MASK)
    {
        
        ON_DEBUG(fprintf(stderr, "## IS REG\n"));
        uint32_t regID = 0;
        memcpy(&regID, Baikal->cmdSheet.buf + Baikal->ip, sizeof(regID));
        if(isAddr)
        {
            addr = Baikal->Reg[regID];
        }
        else
        {
            result = Baikal->Reg[regID];
        }
        Baikal->ip += sizeof(regID);
    }

    if(ArgCode & C_MASK)
    {
        
        ON_DEBUG(fprintf(stderr, "  ## IS C\n"));
        if(isAddr)
        {
            uint32_t taddr = 0;
            memcpy(&taddr, Baikal->cmdSheet.buf + Baikal->ip, sizeof(addr));
            addr += taddr;
            Baikal->ip += sizeof(taddr); 
        }
        else
        {
            double tres = 0;
            memcpy(&tres, Baikal->cmdSheet.buf + Baikal->ip, sizeof(tres));
            result += tres;
            Baikal->ip += sizeof(tres);
        }

    }

    if(ArgCode & RAM_MASK)
    {
        ON_DEBUG(fprintf(stderr, "## IS RAM\n"));
        fprintf(stderr, "RAM addr = %u\n", addr);
        memcpy(&result, Baikal->ram + addr, sizeof(double));
    }
    
    StackPush(Baikal->stk, &result);
    
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