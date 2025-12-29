#include "parse.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    assert(argc == 2 && "Error: Wrong argument count");
    printf("\n\n\n\n\n\nNow assembling %s\n", argv[1]);

    token_block block = read_tokens(argv[1]);

    for (size_t i = 0; i < block.toks.len; ++i)
    {
        token tk = block.toks.data[i];
        printf("%.*s\t", tk.len, tk.ptr);
    }

    printf("finished\n");
    return 0;
}
