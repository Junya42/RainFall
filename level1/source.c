#include <stdio.h>
#include <stdlib.h>

/*
** This function is void because %eax is not set before returning
*/
void run(void) {
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
}

/*
** Implicit return value is 0 because %eax is not set before returning
** So no return value in this case even though the function is int
**
** This might also be caused because of compiler optimization
*/
int main(void) {
	char buffer[64];
	gets(buffer);
}