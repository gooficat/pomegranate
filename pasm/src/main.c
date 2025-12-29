#include "parse.h"
#include "tree.h"
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

    printf("\n");
    asm_tree tree = make_tree(block.toks);

    for (size_t i = 0; i < tree.lines.len; ++i)
    {
        if (tree.lines.data[i].type == LINE_INSTR)
        {
            asm_ins ins = *(asm_ins *)(tree.lines.data[i].ptr);
            printf("instruc '%.*s'\n", ins.name.len, ins.name.ptr);
            for (uint8_t j = 0; j != 2; ++j)
            {
                if (ins.args[j].type)
                {
                    if (ins.args[j].indirection)
                        printf("\tindirect arg\n");
                    else
                        printf("\targ\n");
                }
            }
        }
        else if (tree.lines.data[i].type == LINE_LABEL)
        {
            asm_lab lab = *(asm_lab *)(tree.lines.data[i].ptr);
            printf("label '%.*s'\n", lab.name.len, lab.name.ptr);
        }
    }

    printf("finished\n");
    return 0;
}
