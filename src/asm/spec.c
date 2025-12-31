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
