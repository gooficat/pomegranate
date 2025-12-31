#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAX_ARGS 3
#define MNEM_MAX 8

enum ArgumentSpec {
    NO_ARG = 0,
    
};

enum RegisterType {
    REG_B,
    REG_W,
    REG_D,
    REG_Q,
    REG_SEG,
};

struct Register {
    const char* name;
    uint8_t opcode;
    enum RegisterType type;
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

bool IsInstruction(const char* name);

bool SpecMatch(struct Instruction instruction);

bool OpMatch(struct Instruction instruction, struct Opcode op);

void EncodeInstruction(struct Instruction instruction, struct AssemblyUnit* unit);

struct Opcode* FindOp(struct Instruction instruction);
size_t FindOpIndex(struct Instruction ins);

struct Register* FindReg(const char* name);
size_t FindRegIndex(const char* name);
