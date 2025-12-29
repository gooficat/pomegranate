#pragma once

#include "spec.h"
#include "util.h"

typedef str_view token;

vec_type(token);

class(token_block)
{
    char *cont;
    vector_token toks;
};

token_block read_tokens(const char *path);