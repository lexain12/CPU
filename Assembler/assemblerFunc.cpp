#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../CPU.h"
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

int textToCode(InputFile *inputFile, int *code, Header* header)
{

    Line *_arrayOfLines = inputFile->arrayOfLines;
    char curCmd[MAX_STR_SIZE] = "";
    size_t line  = 0;
    int    num   = 0;
    size_t index = 0;

    while (line < inputFile->numberOfLines)
    {
        sscanf(_arrayOfLines[line].charArray, "%s", curCmd);
        if (strcasecmp(curCmd, "PUSH") == 0)
        {
            sscanf(_arrayOfLines[line].charArray, "%*s %d", &num);
            code[index++] = PUSH_CMD;
            code[index++] = num;
        }
        else if (strcasecmp(curCmd, "ADD") == 0)
        {
            code[index++] = ADD_CMD;
        }
        else if (strcasecmp(curCmd, "SUB") == 0)
        {
            code[index++] = SUB_CMD;
        }
        else if (strcasecmp(curCmd, "MUL") == 0)
        {
            code[index++] = MUL_CMD;
        }
        else if (strcasecmp(curCmd, "DIV") == 0)
        {
            code[index++] = DIV_CMD;
        }
        else if (strcasecmp(curCmd, "OUT") == 0)
        {
            code[index++] = OUT_CMD;
        }
        else if (strcasecmp(curCmd, "HLT") == 0)
        {
            code[index++] = HLT_CMD;
            header->codeSize = index;
            break;
        }
        else 
        {
            return syntaxError;
        }
        line++;
        
   
    }
    if (code[--index] != HLT_CMD)
    {
        printf("No hlt\n");
        return noHltCmd;
    }

    return noErrors;
}
