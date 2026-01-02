#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "mnems.h"

#define MNEM_MAX 24

// this parser is theoretically much simpler than my homebrew-syntax parser.
// also gnu gas is very widespread. Besides, this isnt removing me of the option to revert to that syntax
// ive heard some very vocal complaints about gas and its questionable prefixing syntax

enum ArgumentType
{
    ASM_ARG_NON = 0,
    ASM_ARG_IMM,
    ASM_ARG_REG,
    ASM_ARG_MEM,
};

struct MemoryArgument
{
    struct Register *base;
    struct Register *index;
    uint8_t scale;
    int64_t displacement;
};
struct Argument
{
    enum ArgumentType type;
    union
    {
        struct Register *reg;
        struct MemoryArgument mem;
        uint64_t imm;
        // to be honest, i could just settle for having the memory argument structure
        // for everything and then only doing a union for signed vs unsigned (or casting everywhere)
        // but I heard gnu gas did this and realized it's probably more readable and doesn't really have a performance trade-off
        // or even a size tradeoff
    };
};

// we mark "no args" like a null terminator
// we store the instruction in this structure so that we can match it against a correct mnemonic as per its arguments and then
// assemble it
struct Instruction
{
    char mnemonic[MNEM_MAX];
    struct Argument arguments[MAX_ARGS];
    uint8_t num_args;
};

struct Instruction ParseIns(struct AssemblyState *state);
struct Argument ParseArg(struct AssemblyState *state);

struct MemoryArgument ParseMemArg(struct AssemblyState *state);
bool IsMnemonic(struct AssemblyState *state);

bool IsMnemonic(struct AssemblyState *state);
struct Register *FindRegister(const char *name);
