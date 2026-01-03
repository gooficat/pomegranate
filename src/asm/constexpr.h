#pragma once

#include "asm/asm.h"

#include <stdint.h>

// signed and unsigned

int64_t CompTimeArith(struct AssemblyState *state);
uint64_t UCompTimeArith(struct AssemblyState *state);

int64_t EvalConst(struct AssemblyState *state);
uint64_t UEvalConst(struct AssemblyState *state);
