#pragma once

#include "spec.h"
#include "tree.h"

vec_type(uint8_t);

class(asm_encode_unit)
{
    vector_uint8_t bytes;
    asm_tree tree;
    vector_asm_lab labels;
    bool requires_repass;
};

opcode_s find_opcode(asm_ins ins, asm_encode_unit unit);

asm_encode_unit encode_tree(asm_tree tree);

void encode_ins(asm_encode_unit *unit, size_t i);