#include "tok.h"
#include <stdlib.h>
#include <string.h>
#include "print.h"
#include <stdint.h>

void InitStream(struct TokenStream* stream, const char* path) {
    if (fopen_s(&stream->file, path, "rt")) {
        fprintf(stderr, "Failed to open file %s. exiting...\n", path);
        exit(EXIT_FAILURE);
    }
    stream->buffer = fgetc(stream->file);
    NextToken(stream);
}

void SetStream(struct TokenStream* stream) {
    fseek(stream->file, 0L, SEEK_SET);
    stream->buffer = fgetc(stream->file);
    NextToken(stream);
}

void NextToken(struct TokenStream* stream) {
    while (isspace(stream->buffer))
        stream->buffer = fgetc(stream->file);

    if (stream->buffer == EOF) {
        stream->token[0] = 0;
    }
    else {
        unsigned char i = 0;
        if (stream->buffer == '"') {
            do {
                stream->token[i++] = stream->buffer;
                stream->buffer = fgetc(stream->file);
                
                if (stream->buffer == '\\') {
                    stream->token[i++] = stream->buffer;
                    stream->buffer = fgetc(stream->file);
                }
            } while (stream->buffer != '"');
        }

        if (isalnum(stream->buffer)) {
            do {
                stream->token[i++] = stream->buffer;
                stream->buffer = fgetc(stream->file);
            } while (stream->buffer != EOF && isalnum(stream->buffer));
        }
        else {
            stream->token[0] = stream->buffer;
            ++i;
            stream->buffer = fgetc(stream->file);
        }
        stream->token[i] = 0;
    }
    debug_print("token '%s'\n", stream->token);
}



uint64_t NumberFromToken(char* token) {
    size_t tlen = strlen(token);
    char* end_of_number = token + tlen;
    int radix;
    if (tlen > 2 && isalpha(token[1])) {
        switch (token[1]) {
        case 'x':
            radix = 16;
            break;
        case 'b':
            radix = 2;
            break;
        case 'o':
            radix = 8;
            break;
        case 'z':
            radix = 36;
            break;
        case 'a':
            radix = 26;
            break;
        default:
            radix = token[1] - '0'; // neat!
            break;
        }
        token += 2; // skip over the specifier
    }
    else 
        radix = 10;

    return strtoull(token, &end_of_number, radix);
}