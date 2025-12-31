#include "set.h"


struct Opcode opcodes[] = {
    {""},
};
size_t num_opcodes = sizeof(opcodes) / sizeof(struct Opcode);

struct Register registers[] = {
    {"al", 0x00, REG_B},
    {"cl", 0x01, REG_B},
    {"dl", 0x02, REG_B},
    {"bl", 0x03, REG_B},

    {"ah", 0x04, REG_B},
    {"ch", 0x05, REG_B},
    {"dh", 0x06, REG_B},
    {"bh", 0x07, REG_B},
    
    {"ax", 0x00, REG_W},
    {"cx", 0x01, REG_W},
    {"dx", 0x02, REG_W},
    {"bx", 0x03, REG_W},

    {"sp", 0x04, REG_W},
    {"bp", 0x05, REG_W},
    {"si", 0x06, REG_W},
    {"di", 0x07, REG_W},

    {"eax", 0x00, REG_D},
    {"ecx", 0x01, REG_D},
    {"edx", 0x02, REG_D},
    {"ebx", 0x03, REG_D},

    {"esp", 0x04, REG_D},
    {"ebp", 0x05, REG_D},
    {"esi", 0x06, REG_D},
    {"edi", 0x07, REG_D},

    {"rax", 0x00, REG_Q},
    {"rcx", 0x01, REG_Q},
    {"rdx", 0x02, REG_Q},
    {"rbx", 0x03, REG_Q},

    {"rsp", 0x04, REG_Q},
    {"rbp", 0x05, REG_Q},
    {"rsi", 0x06, REG_Q},
    {"rdi", 0x07, REG_Q},

    {"r8", 0x08, REG_Q},
    {"r9", 0x09, REG_Q},
    {"r10", 0x0A, REG_Q},
    {"r11", 0x0B, REG_Q},

    {"r12", 0x0C, REG_Q},
    {"r13", 0x0D, REG_Q},
    {"r14", 0x0E, REG_Q},
    {"r15", 0x0F, REG_Q},

    {"es", 0x00, REG_SEG},
    {"cs", 0x01, REG_SEG},
    {"ss", 0x02, REG_SEG},
    {"ds", 0x03, REG_SEG},
    {"fs", 0x04, REG_SEG},
    {"gs", 0x05, REG_SEG},

};
size_t num_registers = sizeof(registers) / sizeof(struct Register);