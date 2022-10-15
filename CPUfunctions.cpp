#include <stdio.h>
#include "CPU.h"
#include <math.h>
#include "common.h"
#include <string.h>


void numCpy(void* src, void* dest)
{
    memcpy(dest, src, sizeof(num_t));
}

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
            num_t tnum = NAN;
            numCpy(cpu->code + cpu->ip, &tnum);
            arg += tnum;
            cpu->ip += sizeof(num_t);
        }
        if (cmd & ARG_MEM) arg = cpu->RAM[(int) arg];

        return &arg;
    }
}

void cpuDump(CPU* cpu, Stack_t *stk)
{
    fprintf(DBGFILEPTR, "------------------------------CPU--------------------------------------\n");
    stackDump(stk, 0);
    stackDump(&cpu->LabStk, 0);
    fprintf(DBGFILEPTR, "-----------REGS-------\n");
    for (size_t index = 1; index < 5; ++index)
    {
        fprintf(DBGFILEPTR, "r" "%c" "x: ", 'a' + index - 1);
        fprintf(DBGFILEPTR, Format_ "\n", cpu->Regs[index]);
    }
    fprintf(DBGFILEPTR, "\n----------------------\n");
    fprintf(DBGFILEPTR,   "-----------RAM--------\n");
    for (size_t index = 0; index < 100; ++index) // constantbl
    {
        fprintf(DBGFILEPTR, Format_ " ", cpu->RAM[index]);
        if (index % 10 == 9 )
            fprintf(DBGFILEPTR, "\n");
    }
    fprintf(DBGFILEPTR, "\n----------------------\n");
    fprintf(DBGFILEPTR, "--------------------------END-CPU--------------------------------------\n");
    return ;
}

int execute (CPU* cpu, Header header) 
{
    Stack_t stk = {};
    stackCtor(&stk, 12);
    stackCtor(&(cpu->LabStk), 10);
    cpu->ip = 0;
    num_t *arg = nullptr;

    while (cpu->ip < header.codeSize)
    {
        fprintf(stderr, "iP %02x\n", cpu->ip);
        cpuDump(cpu, &stk);
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
