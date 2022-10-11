#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <string.h>
#include "../common.h"
#include "assembler.h"

static int Labels[12] = {};

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

char checkArg(char* arg, char* Register, num_t *num)
{
    char cmd = 0;
    char* firstChr = strchr(arg, 'r');
    char* lastChr  = strchr(arg, 'x');

    if ((lastChr - firstChr) == 2)
    {
        cmd |= ARG_REG;
        *Register = *(firstChr + 1) - 'a' + 1;
        if (sscanf(arg, "%*[^+]+"Format_, num) > 0) 
        {}
        else if (sscanf(arg, Format_, num) > 0)
        {}
        else
            return cmd;
        cmd |= ARG_IMMED;
        
        return cmd;
    }
    else if (sscanf(arg, ":"Format_, num) > 0)
    {
        cmd |= ARG_IMMED;
        return cmd;
    }
    else if (sscanf(arg, Format_, num))
    {
        cmd |= ARG_IMMED;
        return cmd;
    }
    //else
}

void setArg(char* arg, char* code, size_t* ip, int command)
{
    char  flags    = 0;
    num_t num      = 0;
    char  Register = 0;

    if (arg[0] == '[' && (arg[strlen(arg) - 1]) == ']')
    {
        arg += 1;
        flags |= ARG_MEM;
    }

    flags |= checkArg(arg, &Register, &num);

    code[(*ip)++] = ((char) command) | flags;
    if (flags & ARG_REG) code[(*ip)++] = Register;
    if (strchr(arg, ':'))
    {
        *(num_t*)(code + *ip) = Labels[(int) num];
        (*ip) += sizeof(num_t);
        return;
    }
    if (flags & ARG_IMMED)
    {
        *(num_t*)(code + *ip) = num;
        (*ip) += sizeof(num_t);
    }
}

int textToCode(InputFile *inputFile, char *code, Header* header)
{

    Line *_arrayOfLines = inputFile->arrayOfLines;
    char curCmd[MAX_STR_SIZE] = "";
    char curArg[MAX_STR_SIZE] = "";
    num_t  num      = 0;
    size_t line     = 0;
    size_t ip       = 0;

    while (line < inputFile->numberOfLines)
    {
        sscanf(_arrayOfLines[line].charArray, "%s", curCmd);
        if  (strchr(curCmd, ':'))
        {
            int label = 0;
            sscanf(curCmd, "%d", &label);
            Labels[label] = ip;
        }
#define DEF_CMD(name, num, arg, cod)                                      \ 
        else if (strcasecmp(curCmd, #name) == 0)                            \
        {                                                                   \
            if (arg)                                                        \        
            {                                                               \
                sscanf (_arrayOfLines[line].charArray, "%*s %s", curArg);   \
                setArg (curArg, code, &ip, num);                            \
            }                                                               \        
            else                                                            \
            {                                                               \
                code[ip++] = (char) name##_CMD;                             \
            }                                                               \
        }
#include "../cmd.h"
#undef DEF_CMD
        else if (strcasecmp(curCmd, "HLT") == 0)
        {
            code[ip++] = (char) HLT_CMD;
            header->codeSize = ip;
            break;
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
