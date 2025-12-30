#include "tree.h"
#include "encode.h"
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
            return out;
        }

    return out;
}

size_t find_label(asm_tree *tree, token token)
{
    for (size_t i = 0; i != tree->labs.len; ++i)
        if (token.len == tree->labs.data[i].name.len && !memcmp(token.ptr, tree->labs.data[i].name.ptr, token.len))
            return i;
    printf("label doesnt exist\n");
    return (size_t)-1;
}

asm_arg parse_arg(vector_token tokens, size_t *index, asm_tree *tree)
{
    asm_arg out = {0};
    size_t i = *index;
    if (i >= tokens.len)
        return out;
    if (tokens.data[i].ptr[0] == '$')
    {
        ++i;
        out.value = parse_number(tokens.data[i]);

        out.type = ARG_MEM;

        ++i;
        // todo: check for operator
    }
    else if (isalnum(tokens.data[i].ptr[0]))
    {
        if (isdigit(tokens.data[i].ptr[0]))
            out.value = parse_number(tokens.data[i]);
        else
        {
            if (is_instruction(tokens.data[i]))
            {
                out.type = NO_ARG;
                return out;
            }
            out.references_label = true;
            out.value = find_label(tree, tokens.data[i]);
        }

        out.type = ARG_IMM;

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
        out = parse_arg(tokens, &i, tree);
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
        out.application = new(asm_arg, parse_arg(tokens, &i, tree));
    }

    printf("out type %i\n", out.type);

    *index = i;
    return out;
}

asm_ins *parse_instruction(vector_token tokens, size_t *index, asm_tree *tree)
{
    asm_ins *out = new(asm_ins, {});

    size_t i = *index;
    out->name = tokens.data[i++];

    if (!is_instruction(tokens.data[i]))
    {
        out->args[0] = parse_arg(tokens, &i, tree);
        if (i < tokens.len && tokens.data[i].ptr[0] == ',')
        {
            ++i;
            out->args[1] = parse_arg(tokens, &i, tree);
        }
    }

    if (out->args[0].type != NO_ARG)
    {
        if (out->args[1].type != NO_ARG)
            printf("%.*s has 2 args\n", out->name.len, out->name.ptr);
        else
            printf("%.*s has 1 args\n", out->name.len, out->name.ptr);
    }
    else
        printf("%.*s has 0 args\n", out->name.len, out->name.ptr);

    *index = i;

    return out;
}

const char *asm_direc_names[] = {
    "byte",
    "ascii",
    "times",
};
uint8_t num_asm_direcs = sizeof(asm_direc_names) / sizeof(const char *);

asm_dir *parse_directive(vector_token tokens, size_t *index, asm_tree *tree)
{
    asm_dir *out = new(asm_dir, {.args = make_vec(asm_arg)});
    size_t i = *index;
    ++i;

    for (uint8_t j = 0; j != num_asm_direcs; ++j)
        if (tokens.data[i].len == strlen(asm_direc_names[j]))
        {
            printf("Matches length %hhu", tokens.data[i].len);
            if (!memcmp(tokens.data[i].ptr, asm_direc_names[j], tokens.data[j].len))
            {
                out->type = j;
            }
        }

    printf("directive of type %i aka %s\n", out->type, asm_direc_names[out->type]);
    ++i;

    asm_arg arg;
    while (i < tokens.len)
    {
        arg = parse_arg(tokens, &i, tree);
        if (arg.type == ARG_IMM)
        {
            printf("added immediate to directive args\n");
            push(out->args, arg);
        }
        else
        {
            break;
        }
    }

    *index = i;
    return out;
}

asm_tree make_tree(vector_token tokens)
{
    asm_tree tree = {
        .lines = make_vec(asm_line),
        .labs = make_vec(asm_lab),
    };
    size_t i;
    for (i = 0; i < tokens.len - 1; ++i)
    {
        if (tokens.data[i + 1].ptr[0] == ':')
        {
            push(tree.labs,
                (asm_lab){
                    .name = tokens.data[i],
                    .offset = 0,
                } //
            );
            i += 2;
        }
    }

    i = 0;
    while (i < tokens.len)
    {
        token tk = tokens.data[i];
        asm_line line;

        if (tokens.data[i].ptr[0] == '.')
        {
            printf("direc\n");

            line.type = LINE_DIREC;
            line.ptr = parse_directive(tokens, &i, &tree);
        }
        else if (i < tokens.len - 1 && tokens.data[i + 1].ptr[0] == ':')
        {
            printf("label %.*s\n", tk.len, tk.ptr);
            line.type = LINE_LABEL;
            asm_lab_key k = find_label(&tree, tk);
            printf("label at %i\n", k);
            line.ptr = new(asm_lab_key, k);
            i += 2;
        }
        else
        {
            printf("instr\n");

            line.type = LINE_INSTR;
            line.ptr = parse_instruction(tokens, &i, &tree);
        }
        printf("type %i, val: %.*s\n", line.type, tk.len, tk.ptr);
        push(tree.lines, line);
    }
    return tree;
}

bool is_instruction(token t)
{
    for (size_t i = 0; i != num_ops; ++i)
        if (t.len == strlen(ops[i].name) && !memcmp(ops[i].name, t.ptr, t.len))
            return true;
    return false;
}