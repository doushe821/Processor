#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include "FileManager.h"

int main()
{
    uint32_t Size = 0; 
    if(scanf("%zu", &Size) == 0)
    {
        fprintf(stderr, "Scanning error\n");
        return -1;
    }

    FILE* output = fopen("Circle.wsm", "w+b");
    if(output == NULL)
    {
        fprintf(stderr, "Cant open output file\n");
        return -1;
    }

    for(uint32_t y = 0; y < Size; y++)
    {
        for(uint32_t x = 0; x < Size; x++)
        {
            if((x - Size/2)*(x - Size/2) + (y - Size/2)*(y - Size/2) <= (Size/2)*(Size/2) + 1)
            {
                fprintf(output, "push 1\npop [%u]\n", (y*Size + x)*sizeof(double));
            }
        }
    }
    fprintf(output, "draw %ux%u\n", Size, Size);
    fprintf(output, "hlt\n");
    fclose(output);

    return 0;
}