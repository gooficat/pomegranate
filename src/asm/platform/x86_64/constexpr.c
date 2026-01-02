#include "constexpr.h"

bool IsOperator(const char c)
{
    switch (c)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '|':
    case '^':
    case '&':
        return true;
    default:
        return false;
    }
}

int64_t Charithmetic(int64_t a, const char o, int64_t b)
{
    switch (o)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    case '%':
        return a % b;
    case '|':
        return a | b;
    case '^':
        return a ^ b;
    case '&':
        return a & b;
    }
}

int64_t EvalConst(struct AssemblyState *state)
{

    int64_t base;
    if (state->stream.token[0] == '+')
    {
        NextToken(&state->stream);
    }

    if (state->stream.token[0] == '-')
    {
        NextToken(&state->stream);
        base = -NumberFromToken(state->stream.token); // this looks janky imo
    }
    else if (isdigit(state->stream.token[0]))
    {
        base = NumberFromToken(state->stream.token);
    }
    else if (state->stream.token[0] == '(')
    {
        NextToken(&state->stream); // skip over (
        base = CompTimeArith(state);
    }

    return base;
}

// this function takes the stream's current (hehe) token, looks for an operator (+-*/%&<<>>~|) and if it finds one,
// it fetches the next token, applies it to the base, gets the next token, and repeats
// parentheses just call recursively
int64_t CompTimeArith(struct AssemblyState *state)
{
    int64_t base = EvalConst(state);
    while (IsOperator(state->stream.token[0]))
    {
        char op = state->stream.token[0];
        NextToken(&state->stream);
        int64_t operand = EvalConst(state);
        base = Charithmetic(base, op, operand);
    }
}