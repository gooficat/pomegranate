#include "spec.h"
#include "asm.h"
#include "set.h"
#include <string.h>
// #include

bool IsInstruction(const char* name) {
    for (size_t i = 0; i != num_opcodes; ++i)
        if (!strcmp(name, opcodes[i].mnemonic))
            return true;
    return false;
}

bool SpecMatch(struct Instruction instruction) {
    return true;
}

bool OpMatch(struct Instruction instruction, struct Opcode op) {
    return true;
}

size_t FindOpIndex(struct Instruction ins) {
    for (size_t i = 0; i != num_opcodes; ++i)
        if (OpMatch(ins, opcodes[i]))
            return i;
    return (size_t)-1;
}

struct Opcode* FindOp(struct Instruction ins) {
    size_t i = FindOpIndex(ins);
    return i == (size_t)-1 ? NULL : &opcodes[i];
}

void EncodeInstruction(struct Instruction instruction, struct AssemblyUnit* unit) {
    struct Opcode* op = FindOp(instruction);

    uint8_t opcode = op->opcode;
}

size_t FindRegIndex(const char* name) {
    for (size_t i = 0; i != num_registers; ++i)
        if (!strcmp(name, registers[i].name))
            return i;
    return (size_t)-1;
}

struct Register* FindReg(const char* name) {
    size_t i = FindRegIndex(name);
    return i == (size_t)-1 ? NULL : &registers[i];
}