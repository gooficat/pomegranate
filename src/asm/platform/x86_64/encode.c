#pragma once

#include "asm/asm.h"
#include "print.h"
#include <stdlib.h>

void EncodeInstruction(struct AssemblyState *state)
{
    debug_print("mnemonic %s\n", state->stream.token);
    exit(EXIT_SUCCESS);
}