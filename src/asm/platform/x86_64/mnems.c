#include "mnems.h"

const char *registers[] = {
    "rax",
    "rcx",
    "rdx",
    "rbx",
    "rsp",
    "rbp",
    "rsi",
    "rdi",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15",

};

struct OpProfile instructions[] = {
    {{{'e', 'b'}, {'g', 'b'}, {0}}, 0x00},
    {{{'e', 'v'}, {'g', 'v'}, {0}}, 0x01},
    {{{'g', 'b'}, {'e', 'b'}, {0}}, 0x02},
    {{{'g', 'v'}, {'e', 'v'}, {0}}, 0x03},
    {{{'0', 'l'}, {'i', 'b'}, {0}}, 0x04},
    {{{'0', 'q'}, {'i', 'D'}, {0}}, 0x05},

};

struct Mnemonic mnemonics[] = {
    {"add", 0, 6},
    {"mov", 7, 7},
};
