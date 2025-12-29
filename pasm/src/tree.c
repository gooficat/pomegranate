#include "tree.h"
#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

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

    out = strtoull(bo, &eo, radix);

    return out;
}

asm_arg parse_reg_arg(token tk)
{
    asm_arg out = {0};
    for (uint8_t i = 0; i < num_regs; ++i)
        if (strlen(regs[i].name) == tk.len && !memcmp(tk.ptr, regs[i].name, tk.len))
        {
            out.value = i;
            out.type = ARG_REG;
            printf("reg\n");
            return out;
        }

    return out;
}

asm_arg parse_arg(vector_token tokens, size_t *index)
{
    asm_arg out = {0};
    size_t i = *index;

    if (tokens.data[i].ptr[0] == '$')
    {
        ++i;
        out.value = parse_number(tokens.data[i]);

        out.type = ARG_IMM;

        ++i;
        // todo: check for operator
    }
    else if (isdigit(tokens.data[i].ptr[0]))
    {
        if (isdigit(tokens.data[i].ptr[0]))
            out.value = parse_number(tokens.data[i]);
        else
        {
            out.references_label = true;
        }

        out.type = ARG_MEM;

        ++i;
    }
    else if (tokens.data[i].ptr[0] == '%')
    {
        ++i;
        out = parse_reg_arg(tokens.data[i]);
        ++i;
    }
    else if (tokens.data[i].ptr[0] == '[')
    {
        ++i;
        out = parse_arg(tokens, &i);
        ++out.indirection;
    }
    else
    {
        out.type = NO_ARG;
    }
    while (i < tokens.len && tokens.data[i].ptr[0] == ']')
    {
        // some operands might redirect before the end. some might not. for example, on 6502 we have
        // lda ($LL,X), or in our syntax, something like lda [[$LL, X]]. as you see, it has
        // indirection on the first operand and none on the second
        ++out.redirection;
        ++i;
    }
    if (i < tokens.len && tokens.data[i].len == 1 && tokens.data[i].ptr[0] != '.' && tokens.data[i].ptr[0] != ',')
    {
        out.operation = tokens.data[i].ptr[0];
        ++i;
        out.application = new(asm_arg, parse_arg(tokens, &i));
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
    if (i < tokens.len && tokens.data[i].ptr[0] == ',')
    {
        ++i;
        out->args[1] = parse_arg(tokens, &i);
    }

    if (out->args[0].type != NO_ARG)
    {
        if (out->args[1].type != NO_ARG)
            printf("%.*s has 2 args\n", out->name.len, out->name.ptr);
        else
            printf("%.*s has 1 args\n", out->name.len, out->name.ptr);
    }

    *index = i;

    return out;
}

asm_dir *parse_directive(vector_token tokens, size_t *index)
{
    asm_dir *out = new(asm_dir, {});
    *index += 1;

    out->name = tokens.data[*index];
    // TODO
    // directives like:
    // .db
    // .org

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
            push(tree.labs,
                (asm_lab){
                    .name = tokens.data[i],
                    .offset = 0,
                } //
            );
            line.type = LINE_LABEL;
            line.ptr = new(asm_lab_key, tree.labs.len - 1);
            i += 2;
        }
        else
        {
            line.type = LINE_INSTR;
            line.ptr = parse_instruction(tokens, &i);
        }
        printf("type %i, val: %.*s\n", line.type, tk.len, tk.ptr);
        push(tree.lines, line);
    }
    return tree;
}