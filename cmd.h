#define PUSH(value) stackPush(&stk, value)
#define POP stackPop(&stk)
DEF_CMD(ADD, 2, 0,
{
    PUSH(POP + POP);
    cpu->ip++;
})
DEF_CMD(SUB, 3, 0,
{
    num_t second = POP;
    num_t first = POP;
    PUSH(first - second);
    cpu->ip++;
})
DEF_CMD(MUL, 4, 0,
{
    PUSH(POP * POP);
    cpu->ip++;
})
DEF_CMD(DIV, 5, 0,
{
    num_t second = POP;
    num_t first = POP;
    PUSH(first / second);
    cpu->ip++;
})
DEF_CMD(OUT, 6, 0,
{
    printf("OUT: " Format_ "\n", POP);
    cpu->ip++;
})
DEF_CMD(IN, 7, 0,
{
    num_t tmp = 0;
    scanf(Format_, &tmp);
    PUSH(tmp);
    cpu->ip++;
})
DEF_CMD(POP, 8, 1,
{
    arg = checkArg(cpu);
    *arg = POP;
})
DEF_CMD(PUSH, 1, 1,
{
    arg = checkArg(cpu);
    PUSH(*arg);
})
DEF_CMD(JMP, 9, 1,
{
    arg = checkArg(cpu);
    cpu->ip = (size_t)*arg;
})
DEF_CMD(SQRT, 17, 0,
{
    PUSH(sqrt(POP));
    cpu->ip++;
})
DEF_CMD(CALL, 18, 1,
{
    arg = checkArg(cpu);
    stackPush(&(cpu->LabStk), (double)(cpu->ip));
    cpu->ip = (size_t) *arg;
})
DEF_CMD(RET, 19, 0,
{
    cpu->ip = (size_t) stackPop(&cpu->LabStk);
})
DEF_CMD(VIEW, 20, 0,
{
    printf("\033[2J");
    for (size_t index = 0; index < 225; ++index)
    {
        if (cpu->RAM[index])
            printf("* ");
        else
            printf(". ");
        if (index % 15 == 14)
        {
            printf("\n");
        }
    }
    cpu->ip++;
})
DEF_CMD(IDIV, 21, 0,
{
    num_t second = POP;
    num_t first  = POP;
    PUSH((double)((int)(first / second)));
    cpu->ip++;
})
DEF_CMD(MOD, 24, 0,
{
    num_t second =  POP;
    num_t first  =  POP;
    PUSH(fmod(first, second));
    cpu->ip++;
})
#define DEF_JMP(name, sign, num)          \
DEF_CMD(name, num, 1,                     \
{                                         \
    arg = checkArg(cpu);                  \
    num_t second = POP;                   \
    num_t first  = POP;                   \
    if (first sign second) {              \
        cpu->ip = (size_t) *arg;          \
    }                                     \
})

DEF_JMP(JB, <, 10)
DEF_JMP(JA, >, 11)
DEF_JMP(JE, ==, 12)
DEF_JMP(JBE, <=, 13)
DEF_JMP(JAE, >=, 14)
DEF_JMP(JNE, !=, 15)
#undef DEF_JMP

DEF_CMD(DUMP, 16, 0,
{
    for (size_t index = 0; index < header.codeSize; index++)
    { if (cpu->ip == index)
            printf("[%02X] ", (unsigned int) cpu->code[index]);
        else
            printf("%02X ", (unsigned int) cpu->code[index]);
    }
    printf("\n");
})

#undef PUSH
#undef POP
