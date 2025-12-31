#pragma once

#include <stddef.h>

struct ByteArray {
    unsigned char* bytes;
    size_t num_bytes;
};

struct ByteArray assemble();