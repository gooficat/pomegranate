#include "spec.h"

struct Opcode opcodes[] = {
    {"mov"},
};
size_t num_opcodes = sizeof(opcodes) / sizeof(struct Opcode);

struct Register registers[] = {
    {"r0", 0},
    {"r1", 1},
    {"r2", 2},
    {"r3", 3},
    {"r4", 4},
    {"r5", 5},
    {"r6", 6},
    {"r7", 7},
};
size_t num_registers = sizeof(registers) / sizeof(struct Register);