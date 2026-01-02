#pragma once

#include "asm/asm.h"
#include "tok.h"
#include "print.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "constexpr.h"

#include "mnems.h"
#include "parse.h"

bool MatchInstruction(struct Instruction *ins, struct Mnemonic *mnem)
{
    if (strcmp(ins->mnemonic, mnem->name))
    {
        return false;
    }
    return true;
}

struct Mnemonic *FindMnemonic(struct Instruction *ins)
{
    for (size_t i = 0; i != num_mnemonics; ++i)
    {
        if (MatchInstruction(ins, &mnemonics[i]))
        {
            return &mnemonics[i];
        }
    }

    fprintf(stderr, "No instruction for mnemonic %s\n", ins->mnemonic);

    exit(EXIT_FAILURE);
}

void EncodeInstruction(struct AssemblyState *state)
{
    struct Instruction ins = ParseIns(state);

    struct Mnemonic *mnem = FindMnemonic(&ins);
}
