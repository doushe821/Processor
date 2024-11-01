#include <stdio.h>
#include "FileManager.h"

int main()
{
    const size_t PAGE_SIZE = 96*64;

    FILE* fp = fopen("frames.bin", "r+b");
    if(fp == NULL)
    {
        fprintf(stderr, "Cant open\n");
        return -1;
    }

    FILE* output = fopen("BadApple.wsm", "w+b");

    size_t size = GetFileSize(fp);

    char* buffer = (char*)calloc(size, sizeof(char));
    char subbuffer[PAGE_SIZE] = {};

    fprintf(stderr, "%zu\n", size);
    fread(buffer, size, sizeof(char), fp);
    fclose(fp);

    for(size_t j = 0; j < size; j += PAGE_SIZE)
    {
        fprintf(stderr, "%zu\n", j);
        for(size_t i = 0; i < PAGE_SIZE; i++)
        {
            if(buffer[j + i] == 0)
            {
                if(buffer[j + i] == subbuffer[i])
                {
                    continue;
                }
                fprintf(output, "push 0\npop [%d]\n", i*8);
                subbuffer[i] = 0;
            }
            else
            {
                if(buffer[j + i] == subbuffer[i])
                {
                    continue;
                }
                fprintf(output, "push 1\npop [%d]\n", i*8); 
                subbuffer[i] = 1;
            }
        }
        fprintf(output, "draw 96x64\n");
        fprintf(output, "sleep 100000\n");
    }
    fprintf(output, "hlt\n");
    fclose(output);
    free(buffer);
    return 0;
}