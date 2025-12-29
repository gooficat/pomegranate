#include "encode.h"
#include "spec.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool match_arg(asm_arg arg, asm_arg_spec spec)
{
    if (arg.indirection)
    {
        switch (spec.type)
        {
        case EFF:
        case MEM:
            return true;
        default:
            return false;
        }
    }
    else
        switch (arg.type)
        {
        case ARG_IMM:
            if (spec.type == IMM && arg.value > 0xFF && spec.size == BYT)
                return false;
            return true;

        case ARG_MEM:
            if (spec.type == ABS)
                return true;
            return false;

        case ARG_REG: {
            if (spec.type == GEN || spec.type == EFF)
            {
                if (spec.size == BYT)
                    return regs[arg.value].type == REG_8;
                else
                    return regs[arg.value].type == REG_16;
            }
            else if (spec.type == SEG)
                return regs[arg.value].type == REG_SEG;

            return false;
        }
        break;
        case NO_ARG:
            if (!spec.type)
                return true;
            return false;
        }
}

bool match_spec(asm_ins ins, opcode_s prof)
{
    uint8_t len = strlen(prof.name);
    if (len != ins.name.len || memcmp(ins.name.ptr, prof.name, strlen(prof.name)))
        return false;

    return match_arg(ins.args[0], prof.spec[0]) && match_arg(ins.args[1], prof.spec[1]);
}

opcode_s find_opcode(asm_ins ins)
{
    for (size_t i = 0; i != num_ops; ++i)
        if (match_spec(ins, ops[i]))
            return ops[i];

    return (opcode_s){0};
}

void encode_ins(asm_encode_unit *unit, size_t i)
{
    asm_ins ins = *(asm_ins *)unit->tree.lines.data[i].ptr;
    opcode_s op = find_opcode(ins);
    uint8_t opcode = op.opcode;
    uint8_t modrm = 0;
    bool has_modrm = false;
    uint16_t disp[2] = {0};
    uint8_t displ = 0;

    for (uint8_t i = 0; i != 2; ++i)
    {
        asm_arg arg = ins.args[i];
        if (!arg.type)
            break;
        asm_arg_spec spec = op.spec[i];
        switch (spec.type)
        {
        case EFF:
            if (!arg.indirection)
            {
                has_modrm = true;
                modrm |= regs[arg.value].opcode;
                modrm |= (0b11 << 6);
                printf("added modrm\n");
            }
            break;
        case GEN:
            has_modrm = true;
            modrm |= regs[arg.value].opcode << 3;
            break;
        case IMM:
        case ABS: {
            uint16_t v;
            if (arg.references_label)
                v = unit->labels.data[arg.value].offset;
            else
                v = arg.value;
            disp[displ++] = v & 0xFF;
            if (spec.size != BYT)
                disp[displ++] = (v << 8) & 0xFF;
        }
        break;

        default:
            break;
        }
    }

    printf("0x%02hhx opcode. does%s have modrm which is 0x%hhx. displacement length %hhu\n", opcode,
        has_modrm ? "" : " not", modrm, displ);

    push(unit->bytes, opcode);
    if (has_modrm)
    {
        push(unit->bytes, modrm);
    }

    for (uint8_t i = 0; i != displ; ++i)
    {
        push(unit->bytes, disp[i]);
    }
}

void encode_unit(asm_encode_unit *unit)
{
    unit->requires_repass = false;
    unit->bytes.len = 0;
    for (size_t i = 0; i != unit->tree.lines.len; ++i)
        switch (unit->tree.lines.data[i].type)
        {
        case LINE_INSTR:
            encode_ins(unit, i);
            break;
        case LINE_LABEL: {
            asm_lab_key k = *(asm_lab_key *)unit->tree.lines.data[i].ptr;
            if (unit->labels.data[k].offset != unit->bytes.len)
            {
                unit->requires_repass = true;
            }
            unit->labels.data[k].offset = unit->bytes.len;
        }
        break;
        case LINE_DIREC:
            break;
        }
}

asm_encode_unit encode_tree(asm_tree tree)
{
    asm_encode_unit out = {
        .tree = tree,
        .bytes = make_vec(uint8_t),
        .labels = make_vec(asm_lab),
        .requires_repass = true,
    };

    for (size_t i = 0; i != tree.lines.len; ++i)
        if (tree.lines.data[i].type == LINE_LABEL)
        {
            asm_lab lb = *(asm_lab *)tree.lines.data[i].ptr;
            push(out.labels, lb);
        }

    while (out.requires_repass)
    {
        encode_unit(&out);
    }

    return out;
}
