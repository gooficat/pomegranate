#pragma once

#include <stdbool.h>

struct Mnemonic *FindMnemonic(struct Instruction *ins);

struct OpProfile *FindOpProfile(struct Instruction *ins, struct Mnemonic *mnem);

bool MatchOpcode(struct Instruction *ins, struct OpProfile *op);

bool ArgMatch(struct Argument *argument, struct ArgProfile ins);