#include "spec.h"

struct Opcode opcodes[] = {
    {"mov"},
};
size_t num_opcodes = sizeof(opcodes) / sizeof(struct Opcode);

struct Register registers[] = {
    {""},
};
size_t num_registers = sizeof(registers) / sizeof(struct Register);