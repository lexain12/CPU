#include <stdio.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../common.h"
#include "assembler.h"
#include "LogLib.h"

#ifndef LISTNAME
#define LISTNAME "listing.txt"
#endif

int Labels[12] = {};

FILE* const listOpen(const char*);
FILE* const listOpen(const char* listingFileName)
{
    char buf[0] = {};
    FILE* filePtr = fopen(listingFileName, "w");
    setvbuf(filePtr, buf, _IONBF, 0);

    const time_t timer = time(NULL);
    fprintf(filePtr, "---------------%s", strtok(ctime(&timer), "\n"));
    fseek(filePtr, -1, SEEK_CUR);
    fprintf(filePtr, "---------------\n");

    return filePtr;
}

FILE* const LISTFILEPTR = listOpen(LISTNAME);

void listPrint(void* src, size_t num)
{
    for (size_t index = 0; index < num; ++index)
    {
        char curByte = ((char*)src)[index];
        if (curByte > 0)
            fprintf(LISTFILEPTR, "%02X ", (unsigned int)curByte);
        else 
            fprintf(LISTFILEPTR, "%02X ", (unsigned int) (-1 * curByte));
    }
}

void numCpy(void* src, void* dest)
{
    memcpy(dest, src, sizeof(num_t));
}

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

    arrayOfLines[line].charArray = text;

    line++;

    size_t i = 0;
    for (; text[i + 1] != '\n'; i++)
    {
        if (text[i] == '\0')
        {
           arrayOfLines[line - 1].length    = (size_t) (&text[i] - arrayOfLines[line - 1].charArray);
           arrayOfLines[line]    .charArray = &text[i + 1];

           line++;
        }
    }

    arrayOfLines[line - 1].length = (size_t) (&text[i] - arrayOfLines[line - 1].charArray);
   
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

char checkArg(char* arg, char* Register, num_t *num)
{
    char       cmd = 0;
    char* firstChr = strchr(arg, 'r');
    char* lastChr  = strchr(arg, 'x');

    if ((lastChr - firstChr) == 2)
    {
        cmd |= ARG_REG;
        *Register = *(firstChr + 1) - 'a' + 1;
    
        if (sscanf(arg, "%*[^+]+" Format_, num) > 0) 
        {}
        
        else if (sscanf(arg, Format_, num) > 0)
        {}
        
        else
            return cmd;
        
        cmd |= ARG_IMMED;
        
        return cmd;
    }
    
    else if (sscanf(arg, ":" Format_, num) > 0)
    {
        cmd |= ARG_IMMED;
        return cmd;
    }
    
    else if (sscanf(arg, Format_, num))
    {
        cmd |= ARG_IMMED;
        return cmd;
    }
    
    return cmd;
}

void setArg(char* arg, char* code, size_t* ip, int command)
{
    char  flags    = 0;
    num_t num      = 0;
    char  Register = 0;

    if (arg[0] == '[' && (arg[strlen(arg) - 1]) == ']')
    {
        arg += 1;
        flags |= (char)ARG_MEM;
    }

    flags |= checkArg(arg, &Register, &num);

    char cmd      = (char) command | flags;
    code[(*ip)++] = cmd;
    listPrint(&cmd, sizeof(char));

    if (flags & ARG_REG) 
    {
        code[(*ip)++] = Register;
        listPrint(&Register, sizeof(char));
    }
    
    if (strchr(arg, ':'))
    {
        num_t tnum = (double) Labels[(int) num];
        numCpy(&tnum, (code + *ip));
        (*ip) += sizeof(num_t);

        listPrint(&tnum, sizeof(num_t));

        return;
    }
    
    if (flags & ARG_IMMED)
    {
        numCpy(&num, (code + *ip));
        (*ip) += sizeof(num_t);

        listPrint(&num, sizeof(num_t));
    }
}

int textToCode(InputFile *inputFile, char *code, Header* header)
{

    Line *_arrayOfLines = inputFile->arrayOfLines;
    char curCmd[MAX_STR_SIZE] = "";
    char curArg[MAX_STR_SIZE] = "";
    size_t line     = 0;
    size_t ip       = 0;
    
    fprintf(LISTFILEPTR, "-----------------------------\n");
    while (line < inputFile->numberOfLines)
    {
        sscanf(_arrayOfLines[line].charArray, "%s", curCmd);
  
        if  (strchr(curCmd, ':'))
        {
            int label = 0;
            sscanf(curCmd, "%d", &label);
            Labels[label] = (int) ip;
        }
        
        else if (strchr(curCmd, '/'))
        {
            line++;
            continue;
        }
        
        else if (_arrayOfLines[line].charArray[0] == '\0' || _arrayOfLines[line].charArray[0] == '\n')
        {
            line++;
            continue;
        }

#define DEF_CMD(name, num, arg, cod)                                                                             \
        else if (strcasecmp(curCmd, #name) == 0)                                                                 \
        {                                                                                                        \
            if (arg)                                                                                             \
            {                                                                                                    \
                sscanf (_arrayOfLines[line].charArray, "%*s %s", curArg);                                        \
                fprintf(LISTFILEPTR, "%-6X %-6s %-8s", (unsigned int) ip, curCmd, curArg);                       \
                                                                                                                 \
                setArg (curArg, code, &ip, num);                                                                 \
                fprintf(LISTFILEPTR, "\n");                                                                      \
            }                                                                                                    \
            else                                                                                                 \
            {                                                                                                    \
                fprintf(LISTFILEPTR, "%-6X %-14s %02x\n", (unsigned int) ip, curCmd, (unsigned int) name##_CMD); \
                                                                                                                 \
                code[ip++] = (char) name##_CMD;                                                                  \
            }                                                                                                    \
        }
#include "../cmd.h"
#undef DEF_CMD
        else if (strcasecmp(curCmd, "HLT") == 0)
        {
            code[ip++] = (char) HLT_CMD;
            header->codeSize = ip;
        }
        else 
            return syntaxError;

        line++;
    }

    header->codeSize = ip;
    
    if (code[--ip] != HLT_CMD)
    {
        printf("No hlt\n");
        return noHltCmd;
    }

    return noErrors;
}

void assembly(InputFile *inputFile, char* code, Header *header)
{
    textToCode(inputFile, code, header); 
    textToCode(inputFile, code, header);
}
