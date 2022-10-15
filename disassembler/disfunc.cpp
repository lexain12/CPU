#include <stdio.h>
#include "disfunc.h"

int checkHeader(Header header)
{
    
    if (header.version   != version)
        return versionError;
    if (header.signature != signature)
        return signatureError;
    return noErrors;
}

void checkArg(CPU* cpu)
{
    char arg[12]   = "";
    size_t curChar = 0;
    char curCmd    = cpu->code[cpu->ip++];
    if (curCmd & ARG_MEM)
    {
        arg[curChar++] = '[';
    }
    if (curCmd & ARG_REG)
    {
        arg[curChar++] = 'r';
        arg[curChar++] = 'a' + cpu->code[cpu->ip++] - 1;
        arg[curChar++] = 'x';
        if (curCmd & ARG_IMMED) arg[curChar++] = '+';
    }
    if (curCmd & ARG_IMMED)
    {
        curChar += sprintf(arg+curChar, Format_, *(num_t*)(cpu->code + cpu->ip));
        curChar -= 1;
        cpu->ip += sizeof(num_t);
    }
    if (curCmd & ARG_MEM) arg[curChar++] = ']';
    arg[curChar] = '\0';

    printf("%s\n", arg);

    return;
}

int disasem (CPU* cpu, Header header) 
{
    cpu->ip = 0;
    num_t *arg = nullptr;

    while (cpu->ip < header.codeSize)
    {
        switch (cpu->code[cpu->ip] & CMDMASK)
        {
#define DEF_CMD(name, num, arg, ...) \
            case name##_CMD:         \
                __VA_ARGS__         
#include "cmd.h"
#undef DEF_CMD
            case  HLT_CMD: 
                printf("HLT\n");
            default:
                return disassemError;
            
        }

    }
    
    return disassemError;
}
