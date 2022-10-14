typedef double Elem_t;
typedef unsigned long long Canary_t;
#include "../myStack/stack.h"
#include "CPU.h"
#include <math.h>

int checkHeader(Header header)
{
    
    if (header.version   != version)
        return versionError;
    if (header.signature != signature)
        return signatureError;
    return noErrors;
}

num_t* checkArg(CPU* cpu)
{
    if ((cpu->code[cpu->ip] & CMDMASK) == POP_CMD)
    {
        if (cpu->code[cpu->ip] & ARG_MEM) 
        {
            char cmd  = cpu->code[cpu->ip++];
            num_t arg = 0;

            if (cmd & ARG_REG) arg += cpu->Regs[cpu->code[cpu->ip++]];
            if (cmd & ARG_IMMED) 
            {
                arg     += *(num_t*)(cpu->code + cpu->ip);
                cpu->ip += sizeof(num_t);
            }

            return cpu->RAM + (int) arg;
        }
        else
        {
            char cmd = cpu->code[cpu->ip++];
            num_t* arg = 0;
            if (cmd & ARG_REG) arg = cpu->Regs + (int) cpu->code[cpu->ip++];

            return arg;
        }
    }
    else
    {
        char cmd = cpu->code[cpu->ip++];
        static num_t arg = 0;
        arg = 0;
        if (cmd & ARG_REG) arg += cpu->Regs[cpu->code[cpu->ip++]];
        if (cmd & ARG_IMMED) 
        {
            arg += *(num_t*)(cpu->code + cpu->ip);
            cpu->ip += sizeof(num_t);
        }
        if (cmd & ARG_MEM) arg = cpu->RAM[(int) arg];

        return &arg;
    }
}

int execute (CPU* cpu, Header header) 
{
    Stack_t stk = {};
    stackCtor(&stk, 10);
    cpu->ip = 0;
    num_t *arg = nullptr;

    while (cpu->ip < header.codeSize)
    {
        stackDump(&stk, 0);
        switch (cpu->code[cpu->ip] & CMDMASK)
        {
#define DEF_CMD(name, num, arg, ...) \
            case name##_CMD:         \
                __VA_ARGS__         
#include "cmd.h"
#undef DEF_CMD
            case  HLT_CMD: 
                stackDtor(&stk);
                return 0;
            default:
                return executionError;
            
        }

    }
    return executionError;
}
