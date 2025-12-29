#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void *dump_file(const char *path)
{
    char *cont;
    FILE *f = fopen(path, "rt");
    fseek(f, 0l, SEEK_END);
    long len = ftell(f);
    fseek(f, 0l, SEEK_SET);
    cont = malloc(len + 1);
    size_t nlen = fread(cont, 1, len, f);
    cont[nlen] = 0;
    return cont;
}

token_block read_tokens(const char *path)
{
    token_block out;
    out.toks = make_vec(token);

    out.cont = dump_file(path);

    size_t i = 0;
    while (out.cont[i])
    {
        token tk;
        if (isspace(out.cont[i]))
        {
            ++i;
            continue;
        }
        tk.len = 1;
        tk.ptr = &out.cont[i];

        if (isalnum(out.cont[i]))
            while (isalnum(out.cont[++i]))
                ++tk.len;
        else
            ++i;
        push(out.toks, tk);
    }

    return out;
}