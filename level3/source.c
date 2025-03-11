#include <stdio.h>
#include <stdlib.h>

void v() {
	char buffer[512];
	fgets(buffer, 512, stdin);
	printf(buffer);
	if (1 == 1) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
}

int main() {
	v();
}