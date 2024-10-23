#include "Processor.h"

int main(int argc, char* argv[])
{
    Files files = {};
    files = CmdOpenFile(argc, argv);
    if(files.err)
    {
        return FOPEN_ERROR;
    }

    Header head = {};
    $;
    if(HeaderInit(&head, files.obj) != 0)
    {
        return HEADERINIT_ERROR;
    }
    $;
    Buffer cmdSheet = Bufferise(files.obj, head);
    if(cmdSheet.buf == NULL)
    {      
        return BUFFERISATION_ERROR;
    }
    fclose(files.obj);
    $;
    if(Process(cmdSheet) != 0)
    {
        return COMPILER_ERROR;
    }
    $;
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
    $;
    if(StackInit(&Baikal.stk, 512, sizeof(double)) != 0)
    {
        return STACK_INIT_ERROR;
    }
    Baikal.cmdSheet = buf; 
    $;
    while(true)
    {
        
        //ON_DEBUG(fprintf(stderr, "## OPERATION CODE = %d\n", Baikal.cmdSheet.buf[Baikal.ip]));
        if(doCommand(&Baikal) == HLT)
        {
            break;
        }
        //ON_DEBUG(fprintf(stderr, "## IP = %zu\n", Baikal.ip));
        //usleep(50000);
    }
    StackDtor(Baikal.stk);
    return 0;
}

struct Buffer Bufferise(FILE* obj, const Header head)
{
    struct Buffer Bout = {};

    Bout.buf = (char*)calloc(head.size, sizeof(char));
    Bout.size = head.size;
    if(fread(Bout.buf, Bout.size, sizeof(char), obj) == 0) // replace char 
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
