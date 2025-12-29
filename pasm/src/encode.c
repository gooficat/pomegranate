#include "encode.h"
#include "spec.h"
#include <stdbool.h>
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
            if (spec.type == GEN)
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