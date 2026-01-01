#include "print.h"
#include "asm/asm.h"

int main()
{
	Assemble("../../test.s", "../../test.bin");
	debug_print("Done!");
	return 0;
}
