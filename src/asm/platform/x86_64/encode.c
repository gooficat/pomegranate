#pragma once

#include "asm/asm.h"
#include "tok.h"
#include "print.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "constexpr.h"

#include "mnems.h"

#define MNEM_MAX 24

// find the register to match a name
struct Register *FindRegister(const char *name)
{
    for (int8_t i = 0; i != num_registers; ++i)
    {
        if (!strcmp(name, registers[i].name))
        {
            return &registers[i];
        }
    }
    return NULL;
}

struct MemoryArgument
{
    struct Register *base;
    struct Register *index;
    uint8_t scale;
    int64_t displacement;
};

// we mark "no args" like a null terminator
enum ArgumentType
{
    ASM_ARG_NON = 0,
    ASM_ARG_IMM,
    ASM_ARG_REG,
    ASM_ARG_MEM,
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

// we store the instruction in this structure so that we can match it against a correct mnemonic as per its arguments and then
// assemble it
struct Instruction
{
    char mnemonic[MNEM_MAX];
    struct Argument arguments[MAX_ARGS];
    uint8_t num_args;
};

struct MemoryArgument ParseMemArg(struct AssemblyState *state);
bool IsMnemonic(struct AssemblyState *state);

// this parser is theoretically much simpler than my homebrew-syntax parser.
// also gnu gas is very widespread. Besides, this isnt removing me of the option to revert to that syntax
// ive heard some very vocal complaints about gas and its questionable prefixing syntax
struct Argument ParseArg(struct AssemblyState *state)
{
    struct Argument argument;

    if (state->stream.token[0] == '%')
    {
        argument.type = ASM_ARG_REG;
        NextToken(&state->stream);
        argument.reg = FindRegister(state->stream.token);
    }
    else if (state->stream.token[0] == '$')
    {
        argument.type = ASM_ARG_IMM;
        NextToken(&state->stream);
        argument.imm = EvalConst(state);
    }
    else
    {
        argument.type = ASM_ARG_MEM;
        argument.mem = ParseMemArg(state); // this is frighteningly complicated so ill defer it
    }

    return argument;
}

struct Instruction ParseIns(struct AssemblyState *state)
{
    struct Instruction instruction;

    debug_print("mnemonic %s\n", state->stream.token);
    strcpy(instruction.mnemonic, state->stream.token);
    NextToken(&state->stream);

    while (state->stream.token[0] != '.' && !IsMnemonic(state))
    {
    }

    return instruction;
}

void EncodeInstruction(struct AssemblyState *state)
{
    struct Instruction ins = ParseIns(state);
}

struct MemoryArgument ParseMemArg(struct AssemblyState *state)
{
    struct MemoryArgument out;

    if (isdigit(state->stream.token[0]))
    {
        out.displacement = NumberFromToken(state->stream.token);
    }
    else if (isalpha(state->stream.token[0]))
    {
        out.displacement = FindLabel(state)->offset;
    }
    else
    {
        out.displacement = 0;
    }
    NextToken(&state->stream);

    if (state->stream.token[0] == '(')
    {
        // ouch. this is a lot of token eating. maybe '%' prefix was not such a good idea? might be worth de-implementing that one...
        // something i should probably TODO is a whitespace eater so i can just use the buffer instead of the token sometimes initially
        NextToken(&state->stream);
        if (state->stream.token[0] == '%') // otherwise it's a comma
        {
            NextToken(&state->stream);
            out.base = FindRegister(state->stream.token);
        }
        NextToken(&state->stream); // ,
        NextToken(&state->stream);
        if (state->stream.token[0] == '%') // otherwise it's a comma
        {
            NextToken(&state->stream);
            out.index = FindRegister(state->stream.token);
        }
        NextToken(&state->stream); // , or )
        if (state->stream.token[0] != ')')
        {
            NextToken(&state->stream);
            out.scale = NumberFromToken(state->stream.token);
            NextToken(&state->stream); // ). dont bother arithmeticing this one that would be wacky -- edit: maybe i will
            NextToken(&state->stream);
        }
    }

    return out;
}

bool IsMnemonic(struct AssemblyState *state)
{
    for (size_t i = 0; i != num_mnemonics; ++i)
    {
        if (!strcmp(state->stream.token, mnemonics[i].name))
        {
            return true;
        }
    }
    return false;
}