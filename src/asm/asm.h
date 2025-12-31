#pragma once

#include <stddef.h>

#define LABEL_MAX 32
#define INSTRUC_MAX 32
#define MAX_ARGS 3

struct ByteArray {
    unsigned char* bytes;
    size_t num_bytes;
    size_t capacity;
};

struct ByteArray assemble(const char* file_path);
