#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../CPU.h"
#include "assembler.h"

int main() 
{
    FILE* fileptr = fopen("cmd.txt", "r");
    assert(fileptr != nullptr);

    InputFile inputFile = {};
    int errors          = 0;
    int  *code          = nullptr;
    Header header       = {
        .signature   = (unsigned int) 'VA',
        .version     = 1,
        .codeSize    = 0,
    };

    readFileToLinesStruct(fileptr, &inputFile);
    fclose (fileptr);
    
    code = (int*) calloc(inputFile.numberOfLines * 2, sizeof(int));

    textToCode(&inputFile, code, &header); // make error

    FILE* fileToWrite = fopen("out.bin", "wb");
    assert(fileToWrite != nullptr);
    printf("%lu\n", header.codeSize);
    printf("%u\n", header.signature);
    printf("%lu\n", header.version);
    fwrite (&header, sizeof(Header), sizeof(char), fileToWrite);
    fwrite (code, header.codeSize, sizeof(int), fileToWrite);
    
}

