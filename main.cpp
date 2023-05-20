#include <stdio.h>
#include "common.h"
#include <cassert>
#include "CPU.h"

int main() 
{
    setvbuf(stdout, NULL, _IONBF, 0);
    CPU cpu = {};
    
    FILE *fileToRead = fopen("Assembler/out.bin", "rb");
    assert(fileToRead != nullptr);

    Header header = {};
    size_t numReaded = fread(&header, sizeof(char), sizeof(Header), fileToRead);
    assert(numReaded == sizeof(Header));

    char* code = (char*) calloc(header.codeSize, sizeof(char));
    assert(code != nullptr);
    cpu.code = code;

    numReaded = fread(cpu.code, sizeof(char), header.codeSize, fileToRead);

    execute (&cpu, header);

    //execution

    return 0;
}
