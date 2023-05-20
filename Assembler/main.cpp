#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../common.h"
#include "assembler.h"

int main(int argc, const char* argv[]) 
{
    FILE* fileptr = fopen(argv[1], "r");
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
    
    code = (char*) calloc(inputFile.numberOfLines * 2, sizeof(num_t));

    assembly(&inputFile, code, &header);

    FILE* fileToWrite = fopen("Assembler/out.bin", "wb");
    assert(fileToWrite != nullptr);

    fwrite (&header, sizeof(Header), sizeof(char), fileToWrite);
    fwrite (code, header.codeSize, sizeof(char), fileToWrite);

    return 0;
}

