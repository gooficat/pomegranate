#include "tok.h"
#include <stdlib.h>
#include <string.h>


void InitStream(struct TokenStream* stream, const char* path) {
    if (fopen_s(&stream->file, path, "rt")) {
        fprintf(stderr, "Failed to open file %s. exiting...\n", path);
        exit(EXIT_FAILURE);
    }
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
        if (isalnum(stream->buffer)) {
            do {
                stream->token[i++] = stream->buffer;
                stream->buffer = fgetc(stream->file);
            } while (stream->buffer != EOF && isalnum(stream->buffer));
        }
        else {
            do {
                stream->token[i++] = stream->buffer;
                stream->buffer = fgetc(stream->file);
            } while (stream->buffer != EOF && !isspace(stream->buffer) && !isalnum(stream->buffer));
        }
        stream->token[i] = 0;
    }
}