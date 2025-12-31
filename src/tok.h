#pragma once
#include <stdio.h>

#define TOKEN_MAX 32

struct TokenStream {
    FILE* file;
    int buffer;
    char token[TOKEN_MAX];
};

void InitStream(struct TokenStream* stream, const char* path);

void NextToken(struct TokenStream* stream);