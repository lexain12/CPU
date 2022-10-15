#pragma once
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <cstring>

#ifndef LOGNAME
#define LOGNAME "logfile.txt"
#endif

#include <time.h>
#include <stdlib.h>
#include "LogLib.h"

FILE* const logOpen(const char* logFileName)
{
    char buf[0] = {};
    FILE* filePtr = fopen(logFileName, "a");
    setvbuf(filePtr, buf, _IONBF, 0);

    const time_t timer = time(NULL);
    fprintf(filePtr, "---------------%s", strtok(ctime(&timer), "\n"));
    fseek(filePtr, -1, SEEK_CUR);
    fprintf(filePtr, "---------------\n");

    return filePtr;
}

FILE* const logOpen(const char*);

FILE* const LOGFILEPTR = logOpen(LOGNAME);

tm Tm = {};

int logprint(char param)
{
    fprintf(LOGFILEPTR, "%c", param);
}

int logprint(int param)
{
    fprintf(LOGFILEPTR, "%d", param);
}

int logprint(char* param)
{
    fprintf(LOGFILEPTR, "%s", param);
}

int logprint(double param)
{
    fprintf(LOGFILEPTR, "%lg", param);
}


#define $ fprintf(LOGFILEPTR, "FILE: %s, Function: %s, line: %d\n", __FILE__, __func__, __LINE__);

#define $$(param) param; logprint(#param); logprint(param); logprint('\n'); \

