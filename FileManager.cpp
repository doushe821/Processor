#include "FileManager.h"
size_t GetFileSize(FILE* fp)
{
    
    fseek(fp, 0L, SEEK_END);
    
    size_t size = ftell(fp);
    
    rewind(fp);
    return size;
}