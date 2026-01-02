#include "asm.h"
#include "print.h"
#include <string.h>

#include "platform.h"

void WriteByte(struct AssemblyState *state, uint8_t byte, bool do_write)
{
    ++state->bytes_written;
    if (do_write)
    {
        fputc(byte, state->out);
    }
}

void HandleDirective(struct AssemblyState *state)
{
    NextToken(&state->stream); // .
    debug_print("Directive %s\n", state->stream.token);
    NextToken(&state->stream); // name
    NextToken(&state->stream); // arg
}

void HandleInstruction(struct AssemblyState *state)
{
    EncodeInstruction(state);
}

void HandleAlpha(struct AssemblyState *state)
{
    struct Label *label = FindLabel(state);
    if (label)
    {
        debug_print("Label '%s'\n", state->stream.token);
        if (label->offset != state->bytes_written)
        {
            label->offset = state->bytes_written;
            state->labels_match = false;
        }
        state->stream.buffer = fgetc(state->stream.file);
        NextToken(&state->stream);
    }
    else
    {
        HandleInstruction(state);
    }
}

void Encode(struct AssemblyState *state, bool do_write)
{
    state->labels_match = true;
    while (state->stream.token[0])
    {
        // debug_print("Starting line at token %s\n", state->stream.token);
        if (state->stream.token[0] == '.')
        {
            HandleDirective(state);
        }
        else
        {
            HandleAlpha(state);
        }
    }
}

struct Label *FindLabel(struct AssemblyState *state)
{
    for (uint8_t i = 0; i != state->num_labels; ++i)
    {
        if (!strcmp(state->stream.token, state->labels[i].name))
        {
            return &state->labels[i];
        }
    }
    // debug_print("not a label %s\n", state->stream.token);
    return NULL;
}

void MarkLabels(struct AssemblyState *state)
{
    while (state->stream.token[0])
    {
        if (state->stream.buffer == ':')
        {
            debug_print("Label %s\n", state->stream.token);
            strcpy(state->labels[state->num_labels++].name, state->stream.token);
        }
        NextToken(&state->stream);
    }
}

void Assemble(const char *input_file_path, const char *output_file_path)
{
    struct AssemblyState state;
    InitStream(&state.stream, input_file_path);

    state.num_labels = 0;
    MarkLabels(&state);

    do
    {
        debug_print("Labelling pass\n");
        SetStream(&state.stream);
        Encode(&state, false);
    } while (!state.labels_match);

    SetStream(&state.stream);
    Encode(&state, true);
}