#pragma once

const size_t version         = 1;
const unsigned int signature = 'VA'; 
typedef double num_t;
#define Format_ "%lg"

enum COMMANDS 
{
    HLT_CMD = 0,
#define DEF_CMD(name, num, arg, ...) name##_CMD = num,
#include "cmd.h"
#undef DEF_CMD

};

enum Masks
{
    ARG_IMMED = 1 << 5,
    ARG_REG   = 1 << 6,
    ARG_MEM   = 1 << 7,
    CMDMASK   = (1 << 5) - 1,
};

struct Header
{
    const unsigned int signature;
    const size_t       version;
    size_t             codeSize;
};
