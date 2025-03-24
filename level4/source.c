#include <stdio.h>
#include <stdlib.h>

int m = 0;


void p(char *buffer) {
    printf(buffer);
}

void n(void) {
    char buf[520];

    fgets(buf, 512, stdin);

    p(buf);

    if (m == 16930116) {
        system("/bin/cat /home/user/level5/.pass");
    }
}

int main(void) {
    n();
    return 0;
}
