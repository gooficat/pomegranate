#include "check.h"
#include "tok.h"

bool IsInstruction(const char* name) {
    for (size_t i = 0; i != num_opcodes; ++i)
        if (!strcmp(name, opcodes[i].mnemonic))
            return true;
    return false;
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

size_t FindRegIndex(char* name) {
    if (!isdigit(name[0])) return (size_t)-1;
    size_t n = NumberFromToken(name);
    if (n > 15) return (size_t)-1;
    return n;
}
