#pragma once

#include "parse.h"
#include "spec.h"
#include "util.h"
#include <stdbool.h>

class(asm_arg)
{
    asm_arg_type type;
    uint8_t indirection;
    uint16_t value;
    char operation;
    asm_arg *application;

    bool references_label;
};

class(asm_ins)
{
    str_view name;
    asm_arg args[2];
};

class(asm_lab)
{
    str_view name;
    uint16_t offset;
};

class(asm_dir)
{
    str_view name;
};

vec_type(asm_ins);
vec_type(asm_lab);

typedef void *pvoid;

enum(asm_line_type,
    {
        LINE_INSTR,
        LINE_DIREC,
        LINE_LABEL,
    });

class(asm_line)
{
    asm_line_type type;
    pvoid ptr;
};

vec_type(asm_line);

class(asm_tree)
{
    vector_asm_line lines;
};

asm_tree make_tree(vector_token tokens);