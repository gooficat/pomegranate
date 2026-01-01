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

struct Register {
    const char* name;
    uint8_t opcode;
};

struct Opcode {
    const char* mnemonic;
    uint8_t opcode;
    enum ArgumentSpec args[MAX_ARGS];
};

#define MAX_DISPLACEMENT 8


extern struct Opcode opcodes[];
extern size_t num_opcodes;

extern struct Register registers[];
extern size_t num_registers;

bool SpecMatch(struct Argument arg, enum ArgumentSpec spec);

bool OpMatch(struct Instruction instruction, struct Opcode op);

void EncodeInstruction(struct Instruction instruction, struct AssemblyUnit* unit);
