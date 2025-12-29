#pragma once

#include "util.h"

enum(asm_arg_type,
    {
        NO_ARG = 0,
        ARG_R8,
        ARG_R16,

        ARG_M8,
        ARG_M16,

        ARG_REL8,
        ARG_REL16,

        ARG_SEG,
        ARG_FAR,

        ARG_IMM8,
        ARG_IMM16,
    });

enum(asm_enc_type,
    {
        NOENC = 0,
        ABS,
        EFF,
        GEN,
        IMM,
        REL,
        MEM,
        OFF,
        SEG,
    });

enum(asm_enc_size,
    {
        NOSIZ = 0,
        BYT,
        WOR,
        DEF,
        POI,
        VRI,
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