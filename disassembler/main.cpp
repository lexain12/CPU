#include <stdio.h>
#include "disfunc.h"

int main() 
{
    CPU cpu = {};
    
    FILE *fileToRead = fopen("../Assembler/out.bin", "rb");
    assert(fileToRead != nullptr);

    Header header = {};
    size_t numReaded = fread(&header, sizeof(char), sizeof(Header), fileToRead);
    assert(numReaded == sizeof(Header));

    char* code = (char*) calloc(header.codeSize, sizeof(char));
    assert(code != nullptr);
    cpu.code = code;

    numReaded = fread(cpu.code, sizeof(char), header.codeSize, fileToRead);

    disasem (&cpu, header);

    //execution

    return 0;
}
