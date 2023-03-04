#include <stdio.h>
#include "../includes/sys_get_pid_info.h"

int		main(void) {
	struct pid_info info;

	if (fork()) {
		syscall(548, &info, getpid());
		printf("pid: %u\n", info.pid);
		printf("parent pid: %u\n", info.parent_pid);
	} else {
		sleep(10);
	}
	return (0);
}
