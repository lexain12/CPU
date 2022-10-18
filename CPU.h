#include <cassert>
#include "stack.h"
#include "common.h"

enum CpuErrors
{
    versionError   = 1,
    signatureError = 2,
    executionError = 3,
};

struct CPU 
{
    char* code;
    size_t ip;
    num_t Regs[5] = {};
    num_t RAM[225] = {};
    Stack_t LabStk;
};

void numCpy(char*, char*);
int checkHeader(Header header);
int execute (CPU* cpu, Header header);
num_t* checkArg(CPU* cpu);
