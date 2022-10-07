#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../common.h"
#include "assembler.h"

int main() 
{
    FILE* fileptr = fopen("Assembler/cmd.txt", "r");
    assert(fileptr != nullptr);

    InputFile inputFile = {};
    int errors          = 0;
    char *code          = nullptr;

    Header header       = {
        .signature   = (unsigned int) 'VA',
        .version     = 1,
        .codeSize    = 0,
    };

    readFileToLinesStruct(fileptr, &inputFile);
    fclose (fileptr);
    
    code = (char*) calloc(inputFile.numberOfLines * 2, sizeof(int));

    textToCode(&inputFile, code, &header); // make error

    FILE* fileToWrite = fopen("Assembler/out.bin", "wb");
    assert(fileToWrite != nullptr);

    fwrite (&header, sizeof(Header), sizeof(char), fileToWrite);
    fwrite (code, header.codeSize, sizeof(int), fileToWrite);
    for (size_t index = 0; index < header.codeSize; index++)
    {
        printf("%02X ", code[index]);
    }
    printf("\n");
    
}

