#include <stdio.h>
#include <stdlib.h>

int m = 0x0; //0

void v() {
	char buffer[512];
	fgets(buffer, 512, stdin);
	printf(buffer);
	if (m == 0x40) { //64
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
}

int main() {
	v();
}