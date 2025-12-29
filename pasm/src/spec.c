#include "spec.h"

const reg_s regs8[] = {
    {"al", 0},
    {"cl", 1},
    {"dl", 2},
    {"bl", 3},

    {"ah", 4},
    {"ch", 5},
    {"dh", 6},
    {"bh", 7},
};
const uint8_t num_regs8 = sizeof(regs8) / sizeof(reg_s);
const reg_s regs16[] = {
    {"ax", 0},
    {"cx", 1},
    {"dx", 2},
    {"bx", 3},

    {"sp", 0},
    {"bp", 1},
    {"si", 2},
    {"di", 3},
};
const uint8_t num_regs16 = sizeof(regs16) / sizeof(reg_s);
const reg_s seg_regs[] = {
    {"es", 0},
};
const uint8_t num_seg_regs = sizeof(seg_regs) / sizeof(reg_s);

const opcode_s ops[] = {
    {
        "add",
        0x00,
        {},
    },
};
const uint16_t num_ops = sizeof(ops) / sizeof(opcode_s);