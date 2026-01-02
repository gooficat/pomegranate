#pragma once

#include <stdint.h>

#define MAX_ARGS 3

enum RegisterType
{
    REG_GENQ,
};

struct Register
{
    const char *name;
    uint8_t code;
    enum RegisterType type;
};

struct Mnemonic
{
    const char *name;
    uint8_t code;
};

extern struct Register registers[];

extern uint8_t num_registers;

extern struct Mnemonic mnemonics[];
extern size_t num_mnemonics;