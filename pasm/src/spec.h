#pragma once

#include "util.h"

enum(asm_arg_type,
    {
        NO_ARG = 0,
        ARG_R8 = 1 << 0,
        ARG_R16 = 1 << 1,

        ARG_M8 = 1 << 2,
        ARG_M16 = 1 << 3,

        ARG_REL8 = 1 << 4,
        ARG_REL16 = 1 << 5,

        ARG_SEG = 1 << 6,
        ARG_FAR = 1 << 7,

        ARG_IMM8 = 1 << 8,
        ARG_IMM16 = 1 << 9,
    });

struct
{
    char name[4];
    uint8_t opcode;
} typedef reg_s;

extern const reg_s regs8[];
extern const uint8_t num_regs8;
extern const reg_s regs16[];
extern const uint8_t num_regs16;
extern const reg_s seg_regs[];
extern const uint8_t num_seg_regs;

struct
{
    char name[6];
    uint8_t opcode;
    asm_arg_type spec[2];
} typedef opcode_s;

extern const opcode_s ops[];
extern const uint16_t num_ops;