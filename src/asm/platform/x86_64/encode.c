#include "asm/platform/x86_64/parse.h"

void EncodeInstruction(struct Instruction instruction)
{
    struct OpProfile *prof = FindOpProfile(&instruction, FindMnemonic(&instruction));
}
