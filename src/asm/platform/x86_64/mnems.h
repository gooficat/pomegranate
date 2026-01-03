#pragma once

#include <stdint.h>

#define MAX_ARGS 3

#define BIT(n) (1 << (n))

enum OpType
{
    NO_OP,
    DIR_ADDR,
    DS_ERAX,
    DS_ERBX_AL,
    DS_ERDI,
    CTRL_REG,
    DBG_REG,
    FPU_STI_MEM,
    FPU_STI,
    RFLAGS,
    GEN_MODRM,

};

enum OpSize
{
    SZ_BYTE,
};

struct ArgProfile
{
    char type;
    char size;
};

struct OpProfile
{
    struct ArgProfile profile[MAX_ARGS];
    uint8_t opcode;
};

struct Mnemonic
{
    const char *name;
    uint32_t offset;
    uint16_t num_variants;
};

#define NUM_REGS 16
extern const char *registers[];

extern struct OpProfile instructions[];

#define NUM_MNEMS 2
extern struct Mnemonic mnemonics[2];
