#pragma once

#include "parse.h"
#include "spec.h"
#include "util.h"
#include <stdbool.h>

enum(asm_arg_type,
    {
        NO_ARG = 0,
        ARG_REG,

        ARG_MEM,

        ARG_IMM,
    });

class(asm_arg)
{
    asm_arg_type type;
    uint8_t indirection;
    uint16_t value;
    char operation;
    asm_arg *application;
    uint8_t redirection;
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

enum(asm_direc_type,
    {
        DIREC_BYTE = 0,
        DIREC_ASCI = 1,
        DIREC_TIME = 2,
        DIREC_ORG = 3,
    });

vec_type(asm_arg);

class(asm_dir)
{
    asm_direc_type type;
    vector_asm_arg args;
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

typedef uint16_t asm_lab_key;

vec_type(asm_line);

class(asm_tree)
{
    vector_asm_line lines;
    vector_asm_lab labs;
};

bool is_instruction(token t);

uint16_t parse_number(token tk);

asm_arg parse_arg(vector_token tokens, size_t *index, asm_tree *tree);

asm_ins *parse_instruction(vector_token tokens, size_t *index, asm_tree *tree);

asm_tree make_tree(vector_token tokens);
