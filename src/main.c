#include <stdio.h>
#include "tok.h"
#include "asm/asm.h"

int main() {
	printf("Hello!\n");

	struct ByteArray bytes = assemble("../../test.s");

	return 0;
}
