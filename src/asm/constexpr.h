#pragma once

#include "asm/asm.h"

#include <stdint.h>

int64_t CompTimeArith(struct AssemblyState *state);

int64_t EvalConst(struct AssemblyState *state);