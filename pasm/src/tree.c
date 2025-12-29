#include "tree.h"
#include "parse.h"
#include <ctype.h>

uint16_t parse_number(token tk)
{
    uint16_t out;
    char *bo = tk.ptr;
    char *eo = tk.ptr + tk.len - 1;

    uint8_t radix;

    if (tk.len > 2 && tk.ptr[1] == 'x')
    {
        radix = 16;
        bo += 2;
    }
    else if (tk.len > 2 && tk.ptr[1] == 'b')
    {
        radix = 2;
        bo += 2;
    }
    else
    {
        radix = 10;
    }

    out = strtoull(tk.ptr, &eo, radix);

    return out;
}

asm_arg parse_arg(vector_token tokens, size_t *index)
{
    asm_arg out = {0};
    size_t i = *index;

    if (isdigit(tokens.data[i].ptr[0]))
    {
        out.value = parse_number(tokens.data[i]);

        if (out.value > 0xFF)
            out.type = ARG_IMM16;
        else
            out.type = ARG_IMM8;
    }
    else if (tokens.data[i].ptr[0] == '$')
    {
        ++i;
        if (isdigit(tokens.data[i].ptr[0]))
            out.value = parse_number(tokens.data[i]);
        else
        {
            out.references_label = true;
        }

        if (out.value > 0xFF)
            out.type = ARG_M16;
        else
            out.type = ARG_M8;
    }

    *index = i;
    return out;
}

asm_ins *parse_instruction(vector_token tokens, size_t *index)
{
    asm_ins *out = new(asm_ins, {});

    size_t i = *index;
    out->name = tokens.data[i++];

    out->args[0] = parse_arg(tokens, &i);
    if (tokens.data[i].ptr[0] == ',')
        out->args[1] = parse_arg(tokens, &i);

    *index = i;

    return out;
}

asm_lab *parse_directive(vector_token tokens, size_t *index)
{
    asm_lab *out;
    *index += 1;

    out->name = tokens.data[*index];

    return out;
}

asm_tree make_tree(vector_token tokens)
{
    asm_tree tree = {
        .lines = make_vec(asm_line),
    };
    size_t i = 0;
    while (i < tokens.len)
    {
        token tk = tokens.data[i];
        asm_line line;

        if (tokens.data[i].ptr[0] == '.')
        {
            line.type = LINE_DIREC;
            line.ptr = parse_directive(tokens, &i);
        }
        else if (tokens.data[i + 1].ptr[0] == ':')
        {
            line.type = LINE_LABEL;
            line.ptr = new(asm_lab,
                {
                    .name = tk,
                } //
            );
            i += 2;
        }
        else
        {
            line.type = LINE_INSTR;
            line.ptr = parse_instruction(tokens, &i);
        }
        push(tree.lines, line);
    }
    return tree;
}