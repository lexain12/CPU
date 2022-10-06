const size_t version         = 1;
const unsigned int signature = 'VA'; 

enum COMMANDS 
{
    HLT_CMD  = 0,
    PUSH_CMD = 1,
    ADD_CMD  = 2,
    SUB_CMD  = 3,
    MUL_CMD  = 4,
    DIV_CMD  = 5,
    OUT_CMD  = 6,
};

struct Header
{
    const unsigned int signature;
    const size_t       version;
    size_t             codeSize;
};
