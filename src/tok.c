#include "tok.h"
#include <stdlib.h>
#include <string.h>
#include "print.h"

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
            // do {
            //     stream->token[i++] = stream->buffer;
            //     stream->buffer = fgetc(stream->file);
            // } while (stream->buffer != EOF && !isspace(stream->buffer) && !isalnum(stream->buffer));
            stream->token[i++] = stream->buffer;
            stream->buffer = fgetc(stream->file);
        }
        stream->token[i] = 0;
    }
    debug_print("token '%s'\n", stream->token);
}