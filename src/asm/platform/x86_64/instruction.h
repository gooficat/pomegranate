#pragma once
#include <stdint.h>

#include "tok.h"

#define MNEM_MAX 8
#define ARG_MAX 3

enum ArgumentType
{
    ASM_ARG_IMM,
    ASM_ARG_REG,
    ASM_ARG_MEM,
};

struct Argument
{
    enum ArgumentType type;

    uint8_t base;
    uint8_t index;
    uint8_t scale;
    uint64_t displacement;
};

struct Instruction
{
    char mnemonic[MNEM_MAX];
    struct Argument arguments[ARG_MAX];
};