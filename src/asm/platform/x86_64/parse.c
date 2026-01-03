#include "parse.h"
#include <stdbool.h>
#include <stddef.h>
#include "mnems.h"
#include "asm/asm.h"

#include "print.h"

struct Argument ParseArg(struct AssemblyState *state)
{
    struct Argument argument;

    if (state->stream.token[0] == '%')
    {
        argument.type = ASM_ARG_REG;
        NextToken(&state->stream);
        argument.reg = FindRegister(state->stream.token);
    }
    else if (state->stream.token[0] == '$')
    {
        argument.type = ASM_ARG_IMM;
        NextToken(&state->stream);
        argument.imm = EvalConst(state);
    }
    else
    {
        argument.type = ASM_ARG_MEM;
        argument.mem = ParseMemArg(state); // this is frighteningly complicated so ill defer it
    }
    NextToken(&state->stream);

    // debug_print();

    return argument;
}

struct Instruction ParseInstruction(struct AssemblyState *state)
{
    struct Instruction instruction;

    debug_print("mnemonic %s\n", state->stream.token);
    strcpy(instruction.mnemonic, state->stream.token);
    NextToken(&state->stream);

    while (state->stream.token[0] && state->stream.token[0] != '.' && !IsMnemonic(state))
    {
        instruction.arguments[instruction.num_args] = ParseArg(state);
        instruction.num_args += 1;

        if (state->stream.token[0] != ',')
        {
            break;
        }
        NextToken(&state->stream);
    }

    return instruction;
}

bool IsMnemonic(struct AssemblyState *state)
{
    for (size_t i = 0; i != NUM_MNEMS; ++i)
    {
        if (!strcmp(state->stream.token, mnemonics[i]))
        {
            return true;
        }
    }
    return false;
}

const char *FindRegister(const char *name)
{
    for (int8_t i = 0; i != NUM_REGS; ++i)
    {
        if (!strcmp(name, registers[i]))
        {
            return registers[i];
        }
    }
    return NULL;
}

struct MemoryArgument ParseMemArg(struct AssemblyState *state)
{
    struct MemoryArgument out;

    if (isdigit(state->stream.token[0]))
    {
        out.displacement = NumberFromToken(state->stream.token);
    }
    else if (isalpha(state->stream.token[0]))
    {
        out.displacement = FindLabel(state)->offset;
    }
    else
    {
        out.displacement = 0;
    }
    NextToken(&state->stream);

    if (state->stream.token[0] == '(')
    {
        // ouch. this is a lot of token eating. maybe '%' prefix was not such a good idea? might be worth de-implementing that one...
        // something i should probably TODO is a whitespace eater so i can just use the buffer instead of the token sometimes initially
        NextToken(&state->stream);
        if (state->stream.token[0] == '%') // otherwise it's a comma
        {
            NextToken(&state->stream);
            out.base = FindRegister(state->stream.token);
        }
        NextToken(&state->stream); // ,
        NextToken(&state->stream);
        if (state->stream.token[0] == '%') // otherwise it's a comma
        {
            NextToken(&state->stream);
            out.index = FindRegister(state->stream.token);
        }
        NextToken(&state->stream); // , or )
        if (state->stream.token[0] != ')')
        {
            NextToken(&state->stream);
            out.scale = EvalConst(state);
            NextToken(&state->stream); // ). dont bother arithmeticing this one that would be wacky -- edit: maybe i will
        }
    }

    return out;
}
