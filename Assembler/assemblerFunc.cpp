#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../common.h"
#include "assembler.h"


size_t fileSize (FILE* file)
{
    assert(file != nullptr);

    struct stat filestat = {};
    fstat(fileno(file), &filestat);

    return (size_t) filestat.st_size;
}

int readFile(FILE* openedFile, char** dest)
{
    assert(openedFile != nullptr);
    assert(dest       != nullptr);

    size_t numberOfChars = fileSize(openedFile);

//    printf("filesize = %lu\n", numberOfChars);

    *dest = (char*) calloc(numberOfChars + 1, sizeof(char));

    size_t charsReaded = fread((void*) *dest, sizeof(char), numberOfChars, openedFile); // 
    if (charsReaded != numberOfChars)
        return readingError;
    
    return noErrors;
}

int textToStr(char* text, size_t* numberOfLines)
{
    assert(text          != nullptr);
    assert(numberOfLines != nullptr);

    size_t i = 0; 
    for (; text[i] != '\0'; i++) 
    {
        if (text[i] == '\n')
        {
            *numberOfLines += 1;
            text[i] = '\0';
        }
    }
    
    text[i] = '\n';
//    fprintf(stderr, "number of lines = %lu\n", *numberOfLines);

    return noErrors;
}

int splitIntoLines(InputFile *inputFile)
{
    assert(inputFile != nullptr);
   
    char *text           =  inputFile->text;
    size_t numberOfLines =  inputFile->numberOfLines;
    Line **Lines         = &inputFile->arrayOfLines;


    *Lines = (Line*) calloc(numberOfLines, sizeof(Line));
    Line* arrayOfLines = *Lines;
    
    size_t line = 0;

//    fprintf(stderr, "%s", ((inputFile->arrayOfLines)[line]).charArray);
//    fprintf(stderr, "%s\n", ((arrayOfLines)[line]).charArray);
    arrayOfLines[line].charArray = text;
//    fprintf(stderr, "%s", ((inputFile->arrayOfLines)[line]).charArray);

    line++;

    size_t i = 0;
    for (; text[i + 1] != '\n'; i++)
    {
        if (text[i] == '\0')
        {
//           fprintf(stderr, "line number is %lu\n", line);
           
           arrayOfLines[line - 1].length    = (size_t) (&text[i] - arrayOfLines[line - 1].charArray);
           arrayOfLines[line]    .charArray = &text[i + 1];
           
        // fprintf(stderr, "%s\n", ((arrayOfLines)[line]).charArray);           
 //          fprintf(stderr, "length = %lu\n", arrayOfLines[line - 1].length);
           
           line++;
        }
    }

    arrayOfLines[line - 1].length = (size_t) (&text[i] - arrayOfLines[line - 1].charArray);
   
//    fprintf(stderr, "line number is %lu\n", line);
    
    return noErrors;
}

int readFileToLinesStruct(FILE* openedFile, InputFile* inputFile)
{
    
    Errors err = noErrors;
    if (!err) err = (Errors) readFile(openedFile, &(inputFile->text));
    if (!err) err = (Errors) textToStr(inputFile->text, &(inputFile->numberOfLines));
    if (!err) err = (Errors) splitIntoLines(inputFile);
    
    return err;
}

char checkArg(char* arg, char* Register, int *num)
{
    char cmd = 0;
    char* firstChr = strchr(arg, 'r');
    char* lastChr  = strchr(arg, 'x');

    if ((lastChr - firstChr) == 2)
    {
        cmd |= ARG_REG;
        *Register = *(firstChr + 1) - 'a' + 1;
        if (sscanf(arg, "%*[^+]+%d", num) > 0) 
        {}
        else if (sscanf(arg, "%d", num) > 0)
        {}
        else
            return cmd;
        cmd |= ARG_IMMED;
        
        return cmd;
    }
    else if (sscanf(arg, "%d", num))
    {
        cmd |= ARG_IMMED;
        return cmd;
    }
    //else
}

int textToCode(InputFile *inputFile, char *code, Header* header)
{

    Line *_arrayOfLines = inputFile->arrayOfLines;
    char curCmd[MAX_STR_SIZE] = "";
    size_t line     = 0;
    int    num      = 0;
    size_t ip       = 0;
    size_t numRead  = 0;
    char   Register = 0;

    while (line < inputFile->numberOfLines)
    {
        sscanf(_arrayOfLines[line].charArray, "%s%n", curCmd, &numRead);
        if (strcasecmp(curCmd, "PUSH") == 0)
        {
            char flags = checkArg(_arrayOfLines[line].charArray + numRead, &Register, &num);

            code[ip++] = ((char) PUSH_CMD) | flags;
            if (flags & ARG_REG) code[ip++] = Register;
            if (flags & ARG_IMMED)
            {
                *(int*)(code + ip) = num;
                ip += sizeof(int);
            }
        }
        else if (strcasecmp(curCmd, "POP") == 0)
        {
            char flags = checkArg(_arrayOfLines[line].charArray + numRead, &Register, &num);

            code[ip++] = ((char) POP_CMD) | flags;
            if (flags & ARG_REG) code[ip++] = Register;
        }
        else if (strcasecmp(curCmd, "ADD") == 0)
        {
            code[ip++] = (char) ADD_CMD;
        }
        else if (strcasecmp(curCmd, "SUB") == 0)
        {
            code[ip++] = (char) SUB_CMD;
        }
        else if (strcasecmp(curCmd, "MUL") == 0)
        {
            code[ip++] = (char) MUL_CMD;
        }
        else if (strcasecmp(curCmd, "DIV") == 0)
        {
            code[ip++] = (char) DIV_CMD;
        }
        else if (strcasecmp(curCmd, "OUT") == 0)
        {
            code[ip++] = (char) OUT_CMD;
        }
        else if (strcasecmp(curCmd, "HLT") == 0)
        {
            code[ip++] = (char) HLT_CMD;
            header->codeSize = ip;
            break;
        }
        else if (strcasecmp(curCmd, "IN") == 0)
        {
            code[ip++] = (char) IN_CMD;
        }
        else 
        {
            return syntaxError;
        }
        line++;
        
   
    }
    if (code[--ip] != HLT_CMD)
    {
        printf("No hlt\n");
        return noHltCmd;
    }

    return noErrors;
}
