#include <stdio.h>
#include "CPU.h"

int main() 
{
    int *code        = nullptr;
    FILE *fileToRead = fopen("../Assembler/out.txt", "rb");

    Header header = {};
    size_t numReaded = fread(&header, sizeof(char), sizeof(Header), fileToRead);
    fprintf(stderr, "OK %lu\n", numReaded);
    fprintf(stderr, "%lu\n", header.version);
    fprintf(stderr, "%lu\n", header.codeSize);
    fprintf(stderr, "%u\n", header.signature);
    //read

    //execution

    return 0;
}
