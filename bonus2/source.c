#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lang = 0;   /* 0x8049988 */

void greetuser(char *name)
{
    char greeting[72];          /* ebp-0x48 */

    if (lang == 1)
        strcpy(greeting, "Hyvää päivää ");   /* longest, 0x8048717 */
    else if (lang == 2)
        strcpy(buffer, "Goedemiddag! ");
    else
        strcpy(greeting, "Hello ");          /* 0x8048710 */

    strcat(greeting, name);     /* no bound */
    puts(greeting);
}

int main(int argc, char **argv)
{
    char buf[76];
    char *e;

    if (argc != 3)
        return 1;

    memset(buf, 0, 76);
    strncpy(buf, argv[1], 40);
    strncpy(buf + 40, argv[2], 32);

    e = getenv("LANG");
    if (e && memcmp(e, "fi", 2) == 0)
        lang = 1;
    else if (e && memcmp(e, "nl", 2) == 0)
        lang = 2;

    greetuser(buf);
    return 0;
}