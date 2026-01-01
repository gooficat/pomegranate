#pragma once

#include "tok.h"
#include <stdbool.h>
#include <stdint.h>

#define LABEL_MAX 32
#define DIREC_MAX 7 // .section is the longest afaik

#define MAX_LABELS 255 // seems reasonable, aligns with uint8
#define MAX_RELOCS 255 // no idea about this tbh

struct Label
{
    char name[LABEL_MAX];
    uint64_t offset;
};

struct Directive
{
    char name[DIREC_MAX];
};

struct Relocation
{
    uint64_t offset; //
};

struct AssemblyState
{
    struct TokenStream stream;
    FILE *out;
    struct Label labels[MAX_LABELS];
    struct Relocation Relocations[MAX_RELOCS];
    uint8_t num_labels;
    uint64_t bytes_written;

    bool labels_match;
};

struct Label *FindLabel(struct AssemblyState *state);

void Assemble(const char *input_file_path, const char *output_file_path);