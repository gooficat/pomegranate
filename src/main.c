#include <stdio.h>
#include "tok.h"

int main() {
	printf("Hello!\n");

	struct TokenStream stream;
	InitStream(&stream, "../../test.s");

	while (stream.token[0]) {
		printf("'%s' ", stream.token);
		NextToken(&stream);
	}

	return 0;
}
