#pragma once

#include "asm/asm.h"
#include "tok.h"
#include "print.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "parse.h"

#include "asm/constexpr.h"

#include "asm/platform.h"

bool ArgMatch(struct Argument *argument, struct ArgProfile ins)
{
    // switch ()
    return false;
}

bool MatchOpcode(struct Instruction *ins, struct OpProfile *op)
{
    for (uint8_t i = 0; i != MAX_ARGS; ++i)
    {
        if (!ArgMatch(&ins->arguments[i], op->profile[i]))
        {
            return false;
        }
    }

    return true;
}

struct OpProfile *FindOpProfile(struct Instruction *ins, struct Mnemonic *mnem)
{
    for (size_t i = mnem->offset; i != mnem->offset + mnem->num_variants; ++i)
    {
        if (MatchOpcode(ins, &instructions[i]))
        {
            return &instructions[i];
        }
    }

    fprintf(stderr, "No opcode found for profile of instruction %s\n", ins->mnemonic);

    exit(EXIT_FAILURE);
}

struct Mnemonic *FindMnemonic(struct Instruction *ins)
{
    for (size_t i = 0; i != NUM_MNEMS; ++i)
    {
        if (!strcmp(ins->mnemonic, mnemonics[i].name))
        {
            return &mnemonics[i];
        }
    }

    fprintf(stderr, "No instruction for mnemonic %s\n", ins->mnemonic);

    exit(EXIT_FAILURE);
}
