#include <cassert>
#include "common.h"

enum CpuErrors
{
    versionError   = 1,
    signatureError = 2,
    executionError = 3,
};

int checkHeader(Header header);

int execute (CPU* cpu, Header header);

int* checkArg(CPU* cpu);
