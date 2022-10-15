#pragma once
const int MAX_STR_SIZE = 20;


enum Errors 
{
    noErrors     = 0,
    inputError   = 1,
    readingError = 2,
    noHltCmd     = 3,
    syntaxError  = 4,
};

struct Line
{
    size_t length   = 0;
    char* charArray = nullptr;
};

struct InputFile   
{
    char*  fileName      = nullptr;
    size_t numberOfLines = 0;
    Line*  arrayOfLines  = nullptr;
    char*  text          = nullptr;
};

size_t fileSize (FILE* file);

int readFile(FILE* openedFile, char** dest);

int textToStr(char* text, size_t* numberOfLines);

int splitIntoLines(InputFile *inputFile);

int readFileToLinesStruct(FILE* openedFile, InputFile* inputFile);

int textToCode(InputFile *inputFile, char *code, Header* header);

char checkArg(char* arg, char* Register, num_t *num);

void setArg(char* arg, char* code, size_t* ip, int command);

void assembly(InputFile *inputFile, char *code, Header *header);
