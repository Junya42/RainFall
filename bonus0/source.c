#include <stdlib.h>
#include <string.h>

void p(char *dest, char *msg)
{
    char tmp[4096];

    puts(msg);
    read(0, tmp, 4096);
    *strchr(tmp, '\n') = '\0';
    strncpy(dest, tmp, 20);   /* no NUL if len >= 20 */
}

void pp(char *out)
{
    char a[20];
    char b[20];

    p(a, "-");
    p(b, "-");
    strcpy(out, a);
    /* append 2 bytes from 0x80486a4 */
    strcat(out, b);
}

int main(void)
{
    char buf[42];

    pp(buf);
    puts(buf);
    return 0;
}