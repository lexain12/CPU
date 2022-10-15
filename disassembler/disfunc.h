#include "../common.h"
#include <stdio.h>
#include <cassert>
#include <cstdlib>

int checkHeader(Header header);
void checkArg(CPU* cpu);
int disasem (CPU* cpu, Header header);

enum DisErrors
{
    noErrors       = 0,
    versionError   = 1,
    signatureError = 2,
    disassemError  = 3,
};
