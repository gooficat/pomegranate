#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_ARGS 3
#define MNEM_MAX 8

enum ArgumentSize {
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum ArgumentSpec {
    NO_ARG = 0,
};

struct Opcode {
    const char* mnemonic;
    uint8_t opcode;
    enum ArgumentSpec args[MAX_ARGS];
};

#define MAX_DISPLACEMENT 8

extern struct Opcode opcodes[];
extern size_t num_opcodes;

bool SpecMatch(struct Argument arg, enum ArgumentSpec spec);

bool OpMatch(struct Instruction instruction, struct Opcode op);

void EncodeInstruction(struct Instruction instruction, struct AssemblyUnit* unit);
