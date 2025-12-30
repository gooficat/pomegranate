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

    // add to instruction. this is too general and giving wrong args will cause
    // unexpected behaviour without error, but is here to save time so i can get to testing for now. TODO revisit and
    // add specific options for specific regs
    {"mov", 0xb8, {{ADD, WOR}, {IMM, WOR}}},
    {"mov", 0xb0, {{ADD, BYT}, {IMM, BYT}}},

    {"mov", 0x8e, {{SEG, WOR}, {EFF, WOR}}},

    {"int", 0xcd, {{IMM, BYT}, {0}}},

    {"call", 0xe8, {{REL, VRI}, {0}}},
    {"jmp", 0xeb, {{REL, BYT}, {0}}},
    {"jmp", 0xe9, {{REL, VRI}, {0}}},

    {"cli", 0xFA, {{0}}},
    {"sti", 0xFB, {{0}}},

    {"lodsb", 0xAC, {{0}}},

    {"cmp", 0x3C, {{ADD, BYT}, {IMM, BYT}}},
    {"cmp", 0x3D, {{ADD, VRI}, {IMM, VRI}}},

    {"js", 0x78, {{REL, BYT}, {0}}},
    {"jz", 0x74, {{REL, BYT}, {0}}},

    {"hlt", 0xF4, {{0}}},
};
const uint16_t num_ops = sizeof(ops) / sizeof(opcode_s);