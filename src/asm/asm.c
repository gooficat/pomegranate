#include "asm.h"
#include "tok.h"
#include "print.h"

#include <stdlib.h>
#include <stdbool.h>

struct Label {
    const char name[LABEL_MAX];
    uint64_t offset;
};

struct LabelArray {
    struct Label* labels;
    size_t num_labels;
    size_t capacity;
};

struct AssemblyUnit {
    struct TokenStream stream;
    struct LabelArray labels;
    struct ByteArray bytes;
};

struct Label* FindLabel(const struct LabelArray* labels, const char* name) {
    for (size_t i = 0; i != labels->num_labels; ++i)
        if (!strcmp(labels->labels[i].name, name))
            return &labels->labels[i];
    return NULL;
}

uint64_t NumberFromToken(char* token) {
    char* end_of_number = token + strlen(token);
    return strtoull(token, &end_of_number, 10); // TODO hex, binary
}

uint64_t NumberOrLabel(struct AssemblyUnit* unit) {
    uint64_t out;
    if (isdigit(unit->stream.token[0])) {
        out = NumberFromToken(unit->stream.token);
    }
    else {
        out = FindLabel(&unit->labels, unit->stream.token)->offset;
    }
    return out;
}

struct Argument ParseArgument(struct AssemblyUnit* unit) {
    struct Argument out;
    out.indirection = 0;
    out.redirection = 0;
    while (unit->stream.token[0] == '[') {
        out.indirection += 1;
        NextToken(&unit->stream);
    }

    if (unit->stream.token[0] == '%') {
        NextToken(&unit->stream);
        out.type = ASM_ARG_REG;
        out.value = 67674141;
        // TODO
    }
    else if (unit->stream.token[0] == '$') {
        out.type = ASM_ARG_MEM;

        NextToken(&unit->stream);
        out.value = NumberOrLabel(unit);
    }
    else {
        out.type = ASM_ARG_IMM;
        out.value = NumberOrLabel(unit);
    }
    NextToken(&unit->stream);
    
    // TODO operators

    while (unit->stream.token[0] == ']') {
        out.redirection += 1;
        NextToken(&unit->stream);
    }

    debug_print("\tArgument of type %i and value %llu\n", out.type, out.value);

    return out;
}

inline void ParseDirective(struct AssemblyUnit* unit) {
    debug_print("directive %s\n", unit->stream.token);
    NextToken(&unit->stream);
    NextToken(&unit->stream); // do nothing for now
}

inline void ParseInstruction(struct AssemblyUnit* unit) {
    struct Instruction ins;
    uint8_t num_args = 0;

    debug_print("instruction %s\n", unit->stream.token);
    strcpy(ins.mnemonic, unit->stream.token);
    
    NextToken(&unit->stream);

    while (unit->stream.token[0] && unit->stream.token[0] != '.') {
        ins.args[num_args] = ParseArgument(unit);
        num_args += 1;

        if (unit->stream.token[0] != ',')
            break;
        NextToken(&unit->stream);
    }
}

inline void AddLabel(char* name, struct LabelArray* labels) {
    size_t label_i = labels->num_labels;
    labels->num_labels += 1;

    if (labels->capacity < labels->num_labels) {
        labels->capacity *= 2;
        labels->labels = realloc(labels->labels, sizeof(labels->labels[0]) * labels->capacity);
    }
    strcpy(labels->labels[label_i].name, name);
}

bool encode(struct AssemblyUnit* unit) {
    bool requires_repass = false;
    debug_print("Beginning encode\n\n\n\n\n");
    unit->bytes.num_bytes = 0;

    while (unit->stream.token[0]) {
        // debug_print("%s\n", unit->stream.token);

        if (unit->stream.buffer == ':') {
            struct Label* label = FindLabel(&unit->labels, unit->stream.token);
            if (label->offset != unit->bytes.num_bytes) {
                requires_repass = true;
                label->offset = unit->bytes.num_bytes;
            }
            NextToken(&unit->stream);
            NextToken(&unit->stream);
        }
        
        else if (unit->stream.token[0] == '.') {
            NextToken(&unit->stream);
            ParseDirective(unit);
        }
        
        else {
            ParseInstruction(unit);
        }
    }

    return requires_repass;
}

struct ByteArray assemble(const char* file_path) {
    struct AssemblyUnit unit;


    unit.bytes.bytes = malloc(sizeof(unsigned char));
    unit.bytes.num_bytes = 0;
    unit.bytes.capacity = 1;
    unit.labels.labels = malloc(sizeof(struct Label));
    unit.labels.num_labels = 0;
    unit.labels.capacity = 1;

    InitStream(&unit.stream, file_path);

    // find all the labels
    while (unit.stream.token[0]) {
        if (unit.stream.buffer == ':') {
            debug_print("Added label '%s'\n", unit.stream.token);
            AddLabel(unit.stream.token, &unit.labels);
        }
        NextToken(&unit.stream);
    }

    for (size_t i = 0; i != unit.labels.num_labels; ++i)
        debug_print("unit contains label '%s'", unit.labels.labels[i].name);

    // encode (returning the stream to the beginning each time)
    do
        SetStream(&unit.stream);
    while (encode(&unit));
    

    return unit.bytes;
}
