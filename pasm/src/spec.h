#pragma once

#include "util.h"

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

        ADD,
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

enum(reg_type,
    {
        REG_8,
        REG_16,
        REG_SEG,
    });

class(reg_s)
{
    char name[4];
    uint8_t opcode;
    reg_type type;
};

extern const reg_s regs[];
extern const uint8_t num_regs;

class(asm_arg_spec)
{
    asm_enc_type type;
    asm_enc_size size;
};

class(opcode_s)
{
    char name[6];
    uint8_t opcode;
    asm_arg_spec spec[2];
};

extern const opcode_s ops[];
extern const uint16_t num_ops;

#define i_BX 11
#define i_BP 13
#define i_SI 14
#define i_DI 15