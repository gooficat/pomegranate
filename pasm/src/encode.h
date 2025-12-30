#pragma once

#include "parse.h"
#include "spec.h"
#include "tree.h"

vec_type(uint8_t);

class(asm_encode_unit)
{
    vector_uint8_t bytes;
    asm_tree tree;
    bool requires_repass;
    uint16_t curr_org;
};

opcode_s find_opcode(asm_ins ins, asm_encode_unit unit);

asm_encode_unit encode_tree(asm_tree tree);
void encode_line(asm_encode_unit *unit, size_t *idx);
void encode_ins(asm_encode_unit *unit, size_t i);
