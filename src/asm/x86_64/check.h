#ifdef IGNORETHIS
#pragma once

#include "../asm.h"

bool IsInstruction(const char* name);
struct Opcode* FindOp(struct Instruction instruction);
size_t FindOpIndex(struct Instruction ins);

struct Register* FindReg(const char* name);
size_t FindRegIndex(const char* name);

#endif