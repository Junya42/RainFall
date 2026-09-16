#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int  n;
    char buf[40];                 /* esp+0x14 → esp+0x3c */

    n = atoi(argv[1]);
    if (n > 9)                    /* signed jle */
        return 1;
    memcpy(buf, argv[2], n * 4);
    if (n == 0x574f4c46)          /* "FLOW" */
        execl("/bin/sh", "sh", NULL);
    return 0;
}