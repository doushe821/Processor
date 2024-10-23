#ifndef LABEL_TABLE_INCLUDED
#define LABEL_TABLE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t LABELNAME_MAX = 32;
const size_t LTLENGTH_MAX = 32;

struct label
{
    uint32_t ipTarg;
    char name[LABELNAME_MAX];
};

struct LabelTable
{
    label* labAr;
    size_t lnum;
};


label* LTCtor(label* LT, size_t size);
int LTDtor(label* LT);
int LTDumpf(LabelTable* LT);
int LTDump(LabelTable* LT);
int FindLabel(LabelTable* lt, char* buffer);


#endif