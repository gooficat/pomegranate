#include "mnems.h"

struct Register registers[] = {
    {"rax", 0x00, REG_GENQ},
};
// neat constexpr trick
uint8_t num_registers = sizeof(registers) / sizeof(struct Register);

struct Mnemonic mnemonics[] = {
    {"add", 0x00},
    {"mov", 0x88},
};
size_t num_mnemonics = sizeof(mnemonics) / sizeof(struct Mnemonic);
