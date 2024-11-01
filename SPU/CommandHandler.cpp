#include "Processor.h"


int doCommand(SPU* Baikal)
{
    CommandsStruct[Baikal->cmdSheet.buf[Baikal->ip]].HandleFunc(Baikal);
}



int HandleCall(SPU* Baikal)
{
    Baikal->ip += 1;
    uint32_t ip = 0;
    memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
    Baikal->ip += sizeof(ip);

    StackPush(Baikal->CallStk, &Baikal->ip);

    Baikal->ip = ip;
    return 0;
}

int HandleRet(SPU* Baikal)
{
    uint32_t retIP = 0;
    StackPop(Baikal->CallStk, &retIP);
    Baikal->ip = retIP;
    return 0;
}

int HandlePush(SPU* Baikal)
{
    Baikal->ip++;
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




int HandlePop(SPU* Baikal)
{

    Baikal->ip++;

    uint8_t ArgCode = Baikal->cmdSheet.buf[Baikal->ip];
    
    Baikal->ip++;

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

int HandleAdd(SPU* Baikal)
{
    double a = 0;
    double b = 0;
    StackPop(Baikal->stk, &a);
    StackPop(Baikal->stk, &b);
    double c = a + b; 
    StackPush(Baikal->stk, &c);
    Baikal->ip++;
    return 0;
}

int HandleSub(SPU* Baikal)
{
    double a = 0;
    double b = 0;
    StackPop(Baikal->stk, &a); 
    StackPop(Baikal->stk, &b);
    double c = b - a; 
    StackPush(Baikal->stk, &c);
    Baikal->ip++;
    return 0;
}

int HandleDiv(SPU* Baikal)
{
    double a = 0;
    double b = 0;
    StackPop(Baikal->stk, &a);
    StackPop(Baikal->stk, &b); 
    double c = b / a; 
    StackPush(Baikal->stk, &c); 
    Baikal->ip++;
    return 0;
}

int HandleMul(SPU* Baikal)
{
    double a = 0;
    double b = 0;
    StackPop(Baikal->stk, &a);
    StackPop(Baikal->stk, &b);
    double c = b * a; 
    StackPush(Baikal->stk, &c); 
    Baikal->ip++;
    return 0;
}

int HandlePow(SPU* Baikal)
{
    double a = 0;
    double b = 0;
    StackPop(Baikal->stk, &a);
    StackPop(Baikal->stk, &b); 
    for(int i = 0; i < a; i++)
    {
        b *= b;
    }
    StackPush(Baikal->stk, &b); 
    Baikal->ip++;
    return 0;
}

int HandleSin(SPU* Baikal)
{
    Baikal->ip += 1;
    double a = 0;
    StackPop(Baikal->stk, &a);
    a = sin(a);
    StackPush(Baikal->stk, &a);
    return 0;
}

int HandleOut(SPU* Baikal)
{
    double a = 0;
    StackPop(Baikal->stk, &a);
    printf("%.6lf\n", a);
    Baikal->ip += 1;
    return 0;
}

int HandleJmp(SPU* Baikal)
{
    Baikal->ip += 1;
    uint32_t ip = 0;
    memcpy(&ip, Baikal->cmdSheet.buf + Baikal->ip, sizeof(ip));
    Baikal->ip = ip;
    return 0;
}

int HandleJmb(SPU* Baikal)
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

int HandleJma(SPU* Baikal)
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

int HandleJme(SPU* Baikal)
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

int HandleJmn(SPU* Baikal)
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

int HandleJmbe(SPU* Baikal)
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

int HandleJmae(SPU* Baikal)
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

int HandleSleep(SPU* Baikal)
{
    Baikal->ip += 1;
    uint32_t slpTime = 0;
    memcpy(&slpTime, Baikal->cmdSheet.buf + Baikal->ip, sizeof(slpTime));
    usleep(slpTime);
    Baikal->ip += sizeof(slpTime);
    return 0;
}

int HandleDraw(SPU* Baikal)
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

int HandleIn(SPU* Baikal)
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

int HandleHlt(SPU* Baikal)
{
    return CMD_HLT;
}

int HandleSqrt(SPU* Baikal)
{
    Baikal->ip += 1;
    double a = 0;
    StackPop(Baikal->stk, &a);
    double b = sqrt(a);
    StackPush(Baikal->stk, &b);
    return 0;
}

int HandleUnknown(SPU* Baikal)
{
    fprintf(stderr, "Unknown command\n");
    abort();
    return -1;
}