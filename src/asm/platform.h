#pragma once

struct Argument;
struct Instruction;

#include "platform/x86_64.h"
// #include "platform/armv6_m.h"

void EncodeInstruction(struct Instruction instruction);
struct Instruction ParseInstruction(struct AssemblyState *state);
