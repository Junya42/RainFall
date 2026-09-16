#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void n(void)
{
    system("/bin/cat /home/user/level7/.pass");
}

void m(void)
{
    puts("Nope");
}

int main(int argc, char **argv)
{
    char *buffer;
    void (**func)(void);

    buffer = malloc(64);
    func = malloc(4);
    *func = m;
    strcpy(buffer, argv[1]);
    (*func)();
    return 0;
}