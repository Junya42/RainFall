#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int ac, char **av) {
	av++;
	int num = atoi(*av);

	if (num == 423) {
		char *cmd[2];

		cmd[0] = strdup("/bin/sh");
		cmd[1] = NULL;

		gid_t gid = getegid();
		uid_t uid = geteuid();

		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);

		execv(cmd[0], cmd);
	} else {
		fwrite("No !\n", 1, 5, stderr);
	}
	return 0;
}