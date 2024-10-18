#include "LabelTable.h"


label* LTCtor(label* LT, size_t size)
{
    LT = (label*)calloc(size, sizeof(label));
    if(LT == NULL)
    {
        return NULL;
    }

    for(size_t i = 0; i < size; i++)
    {
        LT[i].ipTarg = -1;
    }
    return LT;
}

int LTDtor(label* LT)
{
    free(LT);
    return 0;
}

int LTDUmp(LabelTable* LT)
{
    FILE* fp =  fopen("01LTDUMP", "w+b"); 
    if(fp == 0)
    {
        fprintf(stderr, "FIle reading error, can't dump\n");
        return -1;
    }

    fprintf(stderr, "sizeof(LT->labAr) / sizeof(label) = %zu\n", sizeof(LT->labAr) / sizeof(label));

    for(size_t i = 0; i < (sizeof(LT->labAr) / sizeof(label)); i++)
    {
        fprintf(fp, "#### [%zu]   %d : \"%32s\"", i, LT->labAr[i].ipTarg, LT->labAr[i].name);
        if(i == LT->lnum)
        {
            fprintf(fp, " <<<<<");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

int FindLabel(LabelTable* lt, char* buffer)
{
    for(size_t i = 0; i < lt->lnum; i++)
    {
        if(strcmp(buffer, lt->labAr[i].name) == 0)
        {
            return lt->labAr[i].ipTarg;
        }
    }
    return -1;
}

