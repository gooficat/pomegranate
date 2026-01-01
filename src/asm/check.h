#pragma once

#include "asm.h"

bool IsInstruction(const char* name);
struct Opcode* FindOp(struct Instruction instruction);
size_t FindOpIndex(struct Instruction ins);

size_t FindRegIndex(char* name);
