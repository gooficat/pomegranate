#pragma once
#include <stdio.h>
#include <stdint.h>

#define TOKEN_MAX 256 // a little big but for string literals it's important

struct TokenStream
{
    FILE *file;
    int buffer;
    char token[TOKEN_MAX];
};

void InitStream(struct TokenStream *stream, const char *path);

void NextToken(struct TokenStream *stream);

void SetStream(struct TokenStream *stream);

uint64_t NumberFromToken(char *token);