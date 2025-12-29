#include "spec.h"

const reg_s regs[] = {
    {"al", 0, REG_8},
    {"cl", 1, REG_8},
    {"dl", 2, REG_8},
    {"bl", 3, REG_8},

    {"ah", 4, REG_8},
    {"ch", 5, REG_8},
    {"dh", 6, REG_8},
    {"bh", 7, REG_8},

    {"ax", 0, REG_16},
    {"cx", 1, REG_16},
    {"dx", 2, REG_16},
    {"bx", 3, REG_16},

    {"sp", 4, REG_16},
    {"bp", 5, REG_16},
    {"si", 6, REG_16},
    {"di", 7, REG_16},

    {"es", 0, REG_SEG},
    {"cs", 1, REG_SEG},
    {"ss", 2, REG_SEG},
    {"ds", 3, REG_SEG},
};
const uint8_t num_regs = sizeof(regs) / sizeof(reg_s);

const opcode_s ops[] = {
    {"add", 0x00, {{EFF, BYT}, {GEN, BYT}}},
    {"add", 0x01, {{EFF, VRI}, {GEN, VRI}}},
    {"add", 0x02, {{GEN, BYT}, {EFF, BYT}}},
    {"add", 0x03, {{GEN, VRI}, {EFF, VRI}}},

    {"mov", 0x88, {{EFF, BYT}, {GEN, BYT}}},
    {"mov", 0x89, {{EFF, VRI}, {GEN, VRI}}},
    {"mov", 0x8a, {{GEN, BYT}, {EFF, BYT}}},
    {"mov", 0x8b, {{GEN, VRI}, {EFF, VRI}}},

    {"call", 0xe8, {{REL, VRI}, {0}}},
    {"jmp", 0xe9, {{REL, VRI}, {0}}},
};
const uint16_t num_ops = sizeof(ops) / sizeof(opcode_s);