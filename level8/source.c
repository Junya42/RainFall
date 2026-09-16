#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *auth = NULL;     /* 0x08049aac */
char *service = NULL;  /* 0x08049ab0 */

int main(void)
{
    char buffer[128];

    while (1)
    {
        printf("%p, %p\n", auth, service);

        if (!fgets(buffer, 128, stdin))
            break;

        if (strncmp(buffer, "auth ", 5) == 0)
        {
            auth = malloc(4);
            *auth = 0;
            if (strlen(buffer + 5) <= 30)
                strcpy(auth, buffer + 5);
        }
        if (strncmp(buffer, "reset", 5) == 0)
            free(auth);
        if (strncmp(buffer, "service", 6) == 0)
            service = strdup(buffer + 7);
        if (strncmp(buffer, "login", 5) == 0)
        {
            if (auth[32])
                system("/bin/sh");
            else
                fwrite("Password:\n", 1, 10, stdout);
        }
    }
    return 0;
}