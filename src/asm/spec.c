#include "spec.h"
#include "asm.h"
#include "check.h"
#include <string.h>
// #include

void EncodeInstruction(struct Instruction instruction, struct AssemblyUnit* unit) {
    struct Opcode* op = FindOp(instruction);

    uint8_t opcode = op->opcode;
}


bool SpecMatch(struct Argument arg, enum ArgumentSpec spec) {
    return true;
}

bool OpMatch(struct Instruction instruction, struct Opcode op) {
    return true;
}
