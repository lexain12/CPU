DEF_CMD(ADD, 2, 0,
{
    printf("%u ", cpu->ip);
    printf("ADD\n");
    cpu->ip++;
})
DEF_CMD(SUB, 3, 0,
{
    printf("%u ", cpu->ip);
    printf("SUB\n");
    cpu->ip++;
})
DEF_CMD(MUL, 4, 0,
{
    printf("%u ", cpu->ip);
    printf("MUL\n");
    cpu->ip++;
})
DEF_CMD(DIV, 5, 0,
{
    printf("%u ", cpu->ip);
    printf("DIV\n");
    cpu->ip++;
})
DEF_CMD(OUT, 6, 0,
{
    printf("%u ", cpu->ip);
    printf("OUT\n");
    cpu->ip++;
})
DEF_CMD(IN, 7, 0,
{
    printf("%u ", cpu->ip);
    printf("IN\n");
    cpu->ip++;
})
DEF_CMD(POP, 8, 1,
{
    printf("%u ", cpu->ip);
    printf("POP ");
    checkArg(cpu);
})
DEF_CMD(PUSH, 1, 1,
{
    printf("%u ", cpu->ip);
    printf("PUSH ");
    checkArg(cpu);
})
DEF_CMD(JMP, 9, 1,
{
    printf("%u ", cpu->ip);
    printf("JMP ");
    checkArg(cpu);
})
DEF_CMD(SQRT, 17, 0,
{
    printf("%u ", cpu->ip);
    printf("SQRT\n");
    cpu->ip++;
})
DEF_CMD(CALL, 18, 1,
{
    printf("%u ", cpu->ip);
    printf("CALL\n");
    checkArg(cpu);
}
DEF_CMD(RET, 19, 0,
{
    printf("%u ", cpu->ip);
    printf("RET\n");
    cpu->ip++;
}
DEF_CMD(VIEW, 20, 0,
{
    printf("%u ", cpu->ip);
    printf("VIEW\n");
    cpu->ip++;
})
DEF_CMD(IDIV, 21, 0,
{
    printf("%u ", cpu->ip);
    printf("IDIV\n");
    cpu->ip++;
})
DEF_CMD(MOD, 24, 0,
{
    printf("%u ", cpu->ip);
    printf("MOD\n");
    cpu->ip++;
})
#define DEF_JMP(name, num) \
DEF_CMD(name, num, 1,            \
{                                \
    printf("%u ", cpu->ip);      \
    printf(#name " ");           \
    checkArg(cpu);               \
    break;                       \
})

DEF_JMP(JB, 10)
DEF_JMP(JA, 11)
DEF_JMP(JE, 12)
DEF_JMP(JBE, 13)
DEF_JMP(JBA, 14)
DEF_JMP(JNE, 15)
#undef DEF_JMP

DEF_CMD(DUMP, 16, 0,
{
    for (size_t index = 0; index < header.codeSize; index++)
    {
        if (cpu->ip == index)
            printf("[%02X] ", cpu->code[index]);
        else
            printf("%02X ", cpu->code[index]);
    }
    printf("\n");
    break;
})
