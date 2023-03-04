#include <stdio.h>
#include "../includes/sys_get_pid_info.h"

int		main(void) {
	uint8_t buffer[200];
	struct pid_info info;

	info.children = (void *)buffer;
	info.children_len = 200;
	if (fork()) {
		syscall(548, &info, getpid());
		printf("pid: %u\n", info.pid);
		printf("parent pid: %u\n", info.parent);
		for (size_t i = 0; i < info.nb_children; i++)
			printf("child[%zu] pid: %u\n", i, info.children[i]);
	} else {
		sleep(10);
	}
	return (0);
}
