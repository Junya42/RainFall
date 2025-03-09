#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void p() {
	fflush(stdout);

	char buffer[76];

	gets(buffer);

	unsigned int return_address = *(unsigned int*)((char *)buffer + 80); //retrieve 4 bytes from the EIP register

	if ((return_address & 0xb0000000) == 0xb0000000) {

		printf("(%p)\n", (void *)return_address);
		exit(1);
	} else {

		puts(buffer);
		strdup(buffer);
	}
}

int main() {

	p();
}