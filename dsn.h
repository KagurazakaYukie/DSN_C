#ifndef DSN_H
#define DSN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MArrayList.h"
#include "mtmm.h"

typedef struct dsnd {
  MArrayList *arraylist;
  MemoryInfo *m,*d,*dn,*n;
} DSN;

typedef bool (*DSNCMP)(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *name,char *q);

DSN *DSNInit(MTMemoryManager *mm);

DSN *Analysis(MTMemoryManager *mm,DSN *dsn,MemoryInfo *d);

void DSNDestroy(MTMemoryManager *mm,DSN *dsn);

MemoryInfo *DSNGetData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *Name);

MemoryInfo *DSNSetData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *nd,MemoryInfo *jdk);

MemoryInfo *DSNDeleteData(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *n);

bool DSNCmp(MTMemoryManager *mm,MemoryInfo *d,MemoryInfo *name,char *q);

bool CharIntervalCmp(char *a,int aq,int aw,char *b);

MemoryInfo *CreateDSNData(MTMemoryManager *mm,char *name,char **dataname,char **data,int size);

int IndexOf(int i,char *a,char b);

bool DSNNameCmp(void *a,void *b);

MemoryInfo *SubString(MTMemoryManager *mm,char *a,int b,int c);

#endif
