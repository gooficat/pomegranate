#include "encode.h"
#include "spec.h"
#include "tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool match_arg(asm_arg arg, asm_arg_spec spec, asm_encode_unit unit)
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
            if (spec.type == IMM)
            {
                // printf("spec imm match\n");
                if (arg.value > 0xFF && spec.size == BYT)
                    return false;
                return true;
            }
            return false;

        case ARG_MEM:
            if (spec.type == ABS)
                return true;
            if (spec.type == REL)
            {
                int16_t loff;
                if (arg.references_label)
                    loff = unit.tree.labs.data[arg.value].offset; // - 2;
                else
                    loff = arg.value;
                if (spec.size == BYT)
                {
                    if (abs((int16_t)loff - (int16_t)unit.bytes.len /* - 3*/) > 0xFF)
                        return false;
                    return true;
                }
                else
                {
                    return true;
                }
            }
            return false;

        case ARG_REG: {
            if (spec.type == GEN || spec.type == EFF || spec.type == ADD)
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

bool match_spec(asm_ins ins, opcode_s prof, asm_encode_unit unit)
{
    // printf("checking spec for %02hhx\n", prof.opcode);

    if (strlen(prof.name) != ins.name.len || memcmp(ins.name.ptr, prof.name, ins.name.len))
        return false;

    return match_arg(ins.args[0], prof.spec[0], unit) && match_arg(ins.args[1], prof.spec[1], unit);
}

opcode_s find_opcode(asm_ins ins, asm_encode_unit unit)
{
    for (size_t i = 0; i != num_ops; ++i)
        if (match_spec(ins, ops[i], unit))
            return ops[i];

    return (opcode_s){0};
}

void encode_ins(asm_encode_unit *unit, size_t i)
{
    asm_ins ins = *(asm_ins *)unit->tree.lines.data[i].ptr;
    opcode_s op = find_opcode(ins, *unit);
    uint8_t opcode = op.opcode;
    uint8_t modrm = 0;
    bool has_modrm = false;
    uint16_t disp[2] = {0};
    uint8_t displ = 0;

    for (uint8_t i = 0; i != 2; ++i)
    {
        asm_arg arg = ins.args[i];
        printf("Arg %i for %.*s does %sref label\n", i, ins.name.len, ins.name.ptr, arg.references_label ? "" : "not ");
        if (!arg.type)
            break;
        asm_arg_spec spec = op.spec[i];
        switch (spec.type)
        {
        case GEN:
            has_modrm = true;
            modrm |= regs[arg.value].opcode << 3;
            break;
        case IMM:
        case ABS: {
            uint16_t v;
            if (arg.references_label)
                v = unit->tree.labs.data[arg.value].offset;
            else
                v = arg.value;

            v += unit->curr_org;

            disp[displ++] = v & 0xFF;
            if (spec.size != BYT)
                disp[displ++] = (v >> 8) & 0xFF;
        }
        break;
        case EFF:
            has_modrm = true;
            if (!arg.indirection)
            {
                modrm |= regs[arg.value].opcode;
                modrm |= (0b11 << 6);
                printf("added modrm\n");
            }
            else
            {
                asm_arg *parg = &arg;
                uint8_t base = 0;
                uint8_t index = 0;
                uint16_t displacement = 0;

                if (parg->type == ARG_REG && (parg->value == i_BP || parg->value == i_BX))
                {
                    base = parg->value;
                    parg = parg->application;
                    printf("base\n");
                }
                if (parg && parg->type == ARG_REG)
                {
                    index = parg->value;
                    parg = parg->application;
                    printf("index\n");
                }
                if (parg && parg->type == ARG_IMM)
                {
                    if (parg->references_label)
                    {
                        displacement = unit->tree.labs.data[parg->value].offset;
                        printf("index label\n");
                    }
                    else
                    {
                        displacement = parg->value;
                        printf("imm\n");
                    }
                    do
                    {
                        asm_arg prev_arg = arg;
                        if (parg->operation)
                            parg = parg->application;
                        else
                            break;

                        uint16_t v;
                        if (parg->references_label)
                            v = unit->tree.labs.data[parg->value].offset;
                        else
                        {
                            v = parg->value;
                            printf("arg value of %hu\n", v);
                        }

                        switch (prev_arg.operation)
                        {
                        case '*':
                            displacement *= v;
                            break;
                        case '+':
                            displacement += v;
                            break;
                        case '-':
                            displacement -= v;
                            break;
                        case '/':
                            displacement /= v;
                            break;
                        default:
                            break;
                        }
                    } while (parg && parg->type == ARG_IMM);
                }
                if (!base)
                {
                    modrm |= index == i_SI ? 0b100 : 0b101;
                }
                else if (base == i_BP)
                    modrm |= 0b10;

                if (!index)
                {
                    modrm |= base != i_BX ? 0b110 : 0b111;
                }
                if (index == i_DI)
                    modrm |= 0b1;

                uint8_t mod = 0;
                if (displacement)
                {
                    disp[displ++] = displacement & 0xFF;
                    ++mod;
                }
                if (displacement > 0xFF)
                {
                    disp[displ++] = (displacement >> 8) & 0xFF;
                    ++mod;
                }
                modrm |= (mod << 6);

                printf("modrm is %hhx disp is %hu mod is %hhx\n", modrm, displacement, mod);
            }
            break;
        case REL: {
            uint16_t v;
            uint16_t o;
            if (arg.references_label)
            {
                v = unit->tree.labs.data[arg.value].offset;
                printf("Refs label %.*s\n", unit->tree.labs.data[arg.value].name.len,
                    unit->tree.labs.data[arg.value].name.ptr);
            }
            else
            {
                v = arg.value;
                printf("no label ref\n");
            }
            if (spec.size != BYT)
                o = v - unit->bytes.len - 3; // - 3;
            else
                o = v - unit->bytes.len - 2;

            printf("calculated offset for ins ending at %llu and abs addr is %hu, as being %04hx\n", unit->bytes.len, v,
                o);

            disp[displ++] = o & 0xFF;
            if (spec.size != BYT)
                disp[displ++] = (o >> 8) & 0xFF;
        }
        break;
        case ADD:
            opcode += regs[arg.value].opcode;
            break;
        default:
            break;
        }
    }

    // printf("0x%02hhx opcode. does%s have modrm which is 0x%hhx. displacement length %hhu\n", opcode,
    // has_modrm ? "" : " not", modrm, displ);

    push(unit->bytes, opcode);
    if (has_modrm)
        push(unit->bytes, modrm);

    for (uint8_t i = 0; i != displ; ++i)
        push(unit->bytes, disp[i]);
}

uint16_t assemble_time_math(asm_arg arg, asm_encode_unit *unit)
{
    uint16_t num;
    if (arg.references_label)
        num = unit->tree.labs.data[arg.value].offset;
    else
        num = arg.value;

    char op = arg.operation;
    while (arg.application)
    {
        arg = *arg.application;
        uint16_t v;
        if (arg.references_label)
            v = unit->tree.labs.data[arg.value].offset;
        else
            v = arg.value;

        switch (op)
        {
        case '+':
            num += v;
            break;
        case '-':
            num -= v;
            break;
        case '*':
            num *= v;
        case '/':
            num /= v;
            break;
        }
        op = arg.operation;
    }
    return num;
}

void encode_direc(asm_encode_unit *unit, size_t *idx)
{
    size_t i = *idx;
    asm_dir dir = *(asm_dir *)(unit->tree.lines.data[i].ptr);

    asm_arg arg = dir.args.data[0];
    // printf("directive of type %i and arg 1 has val %hu\n", dir.type, arg.value);
    switch (dir.type)
    {
    case DIREC_TIME: {
        uint16_t num = assemble_time_math(arg, unit);
        size_t pi = ++i;
        while (num > 0)
        {
            i = pi;
            encode_line(unit, &i);
            --num;
        }
    }
    break;
    case DIREC_BYTE: {
        for (size_t j = 0; j < dir.args.len; ++j)
        {
            push(unit->bytes, arg.value);
            arg = dir.args.data[j];
        }
        ++i;
    }
    break;
    case DIREC_ASCI: {
    }
    break; // TODO
    }
    *idx = i;
}

void encode_line(asm_encode_unit *unit, size_t *idx)
{
    size_t i = *idx;
    switch (unit->tree.lines.data[i].type)
    {
    case LINE_INSTR:
        encode_ins(unit, i);
        ++i;
        break;
    case LINE_LABEL: {
        asm_lab_key k = *(asm_lab_key *)unit->tree.lines.data[i].ptr;
        // printf("encountered label %.*s\n", unit->tree.labs.data[k].name.len, unit->tree.labs.data[k].name.ptr);
        printf("Label contents vs byte offset. %hu vs %llu\n", unit->tree.labs.data[k].offset, unit->bytes.len);
        if (unit->tree.labs.data[k].offset != unit->bytes.len)
        {
            unit->requires_repass = true;
            unit->tree.labs.data[k].offset = unit->bytes.len;
        }
        ++i;
    }
    break;
    case LINE_DIREC:
        encode_direc(unit, &i);
        break;
    }
    *idx = i;
}

void encode_unit(asm_encode_unit *unit)
{
    unit->requires_repass = false;
    unit->bytes.len = 0;
    size_t i = 0;
    while (i < unit->tree.lines.len)
        encode_line(unit, &i);
}

asm_encode_unit encode_tree(asm_tree tree)
{
    asm_encode_unit out = {
        .tree = tree,
        .bytes = make_vec(uint8_t),
        .requires_repass = true,
        .curr_org = 0,
    };

    while (out.requires_repass)
    {
        encode_unit(&out);
    }

    return out;
}
