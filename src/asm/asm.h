#pragma once

#include <stddef.h>
#include <stdint.h>
#include "spec.h"

#define LABEL_MAX 32

struct ByteArray {
    unsigned char* bytes;
    size_t num_bytes;
    size_t capacity;
};


enum ArgumentType {
    ASM_ARG_IMM,
    ASM_ARG_ASC,
    ASM_ARG_REG,
    ASM_ARG_MEM,
};

struct Argument {
    enum ArgumentType type;
    union {
        uint64_t value;
        char* str_value; // for ascii arg. directive only
    };
    uint8_t indirection; // how many degrees of indirection ([ before the arg)
    uint8_t redirection; // how many ] after the arg
    char operation; // preceding operation (+-/*%), if 0 then no operand
};


struct Instruction {
    char mnemonic[MNEM_MAX];
    struct Argument args[MAX_ARGS];
};

struct Directive {
    char mnemonic[MNEM_MAX];
    struct Argument *args;
    uint16_t num_args;
};

struct ByteArray assemble(const char* file_path);

