#include "encode.h"
#include "parse.h"
#include "tree.h"
#include <assert.h>
#include <stdio.h>

void print_arg(asm_arg arg)
{
    printf("\t");
    if (arg.indirection)
        printf("in");
    printf("direct arg. type %i, value %hu \n", arg.type, arg.value);
    if (arg.operation)
    {
        printf("\t%c", arg.operation);
        print_arg(*arg.application);
    }
    printf("\n");
}

void print_instr(asm_ins ins)
{
    printf("instruc '%.*s'\n", ins.name.len, ins.name.ptr);
    for (uint8_t j = 0; j != 2; ++j)
    {
        if (ins.args[j].type)
        {
            print_arg(ins.args[j]);
        }
    }
}

int main(int argc, char *argv[])
{
    assert(argc == 3 && "Error: Wrong argument count");
    printf("\n\n\n\n\n\nNow assembling %s\n", argv[1]);

    token_block block = read_tokens(argv[1]);

    for (size_t i = 0; i < block.toks.len; ++i)
    {
        token tk = block.toks.data[i];
        printf("%.*s\t", tk.len, tk.ptr);
    }

    printf("\n");
    asm_tree tree = make_tree(block.toks);

    asm_encode_unit encoded = encode_tree(tree);
    // for (size_t i = 0; i < tree.lines.len; ++i)
    // {
    //     if (tree.lines.data[i].type == LINE_INSTR)
    //     {
    //         asm_ins ins = *(asm_ins *)(tree.lines.data[i].ptr);
    //         print_instr(ins);

    //         opcode_s op = find_opcode(ins, encoded);
    //         printf("instruc %.*s matches opcode %hhu\n", ins.name.len, ins.name.ptr, op.opcode);
    //     }
    //     else if (tree.lines.data[i].type == LINE_LABEL)
    //     {
    //         asm_lab lab = tree.labs.data[*(asm_lab_key *)(tree.lines.data[i].ptr)];
    //         printf("label '%.*s'\n", lab.name.len, lab.name.ptr);
    //     }
    // }

    for (size_t i = 0; i != encoded.tree.labs.len; ++i)
        printf("label is offset %hu\n", encoded.tree.labs.data[i].offset);

    for (size_t i = 0; i != encoded.bytes.len; ++i)
        printf("%02hhx ", encoded.bytes.data[i]);

    FILE *out = fopen(argv[2], "wb");
    fwrite(encoded.bytes.data, 1, encoded.bytes.len, out);
    printf("\nwrote %llu bytes to %s\n", encoded.bytes.len, argv[2]);
    fclose(out);

    printf("finished\n");
    return 0;
}
