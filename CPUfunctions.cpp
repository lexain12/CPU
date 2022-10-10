typedef int Elem_t;
typedef unsigned long long Canary_t;
#include "../myStack/stack.h"
#include "CPU.h"

int checkHeader(Header header)
{
    
    if (header.version   != version)
        return versionError;
    if (header.signature != signature)
        return signatureError;
    return noErrors;
}

int* checkArg(CPU* cpu)
{
    if (cpu->code[cpu->ip] == POP_CMD)
    {
        printf("here1\n");
        fprintf(stderr, "%02X\n", cpu->code[cpu->ip]);
        if (cpu->code[cpu->ip] & ARG_MEM) 
        {
            fprintf(stderr, "%02X\n", cpu->code[cpu->ip]);
            char cmd = cpu->code[cpu->ip++];
            fprintf(stderr, "%02X\n", cpu->code[cpu->ip]);
            int arg = 0;

            if (cmd & ARG_REG) arg += cpu->Regs[cpu->code[cpu->ip++]];
            if (cmd & ARG_IMMED) 
            {
                arg += *(int*)(cpu->code + cpu->ip);
                cpu->ip += sizeof(int);
            }
            fprintf(stderr, "%02X\n", cpu->code[cpu->ip]);

            return cpu->RAM + arg;
        }
        else
        {
            char cmd = cpu->code[cpu->ip++];
            int* arg = 0;
            if (cmd & ARG_REG) arg = cpu->Regs + cpu->code[cpu->ip++];

            return arg;
        }
    }
    else
    {
        printf("here\n");
        char cmd = cpu->code[cpu->ip++];
//        fprintf(stderr, "%02x\n", cmd);
        static int arg = 0;
        arg = 0;
        if (cmd & ARG_REG) arg += cpu->Regs[cpu->code[cpu->ip++]];
        if (cmd & ARG_IMMED) 
        {
            arg += *(int*)(cpu->code + cpu->ip);
            cpu->ip += sizeof(int);
        }
        fprintf(stderr, "1 %d\n", arg);
        if (cmd & ARG_MEM) arg = cpu->RAM[arg];
        fprintf(stderr, "2 %d\n", arg);

        return &arg;
    }
}

int execute (CPU* cpu, Header header) 
{
    Stack_t stk = {};
    stackCtor(&stk, 10);
    cpu->ip = 0;
    int *arg = nullptr;

    while (cpu->ip < header.codeSize)
    {
        stackDump(&stk, 0);
        switch (cpu->code[cpu->ip] & CMDMASK)
        {
#define DEF_CMD(name, num, arg, ...)\
            case name##_CMD:        \
                __VA_ARGS__         \
                break;
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
