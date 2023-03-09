#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../includes/sys_get_pid_info.h"

int		main(int argc, char *argv[]) {
	int					ret;
	pid_t				pid;
	struct pid_info		info;

	if (argc != 2) {
		dprintf(STDERR_FILENO, "usage: %s PID\n", argv[0]);
		return (1);
	}
	bzero(&info, sizeof(struct pid_info));
	info.children = malloc(4096);
	info.exe = malloc(4096);
	info.root = malloc(4096);
	info.pwd = malloc(4096);
	if (!info.children || !info.exe || !info.root || !info.pwd) {
		dprintf(STDERR_FILENO, "%s: malloc error", argv[0]);
		free(info.children);
		free(info.exe);
		free(info.root);
		free(info.pwd);
		return (1);
	}
	info.children_len = 4096;
	pid = atoi(argv[1]);
	ret = syscall(__NR_get_pid_info, &info, pid);
	if (ret) {
		dprintf(STDERR_FILENO, "%s: [Errno %d]: %s\n", argv[0], errno, strerror(errno));
		free(info.children);
		free(info.exe);
		free(info.root);
		free(info.pwd);
		return (ret);
	}
	printf("Process Information:\n");
	printf("pid: %u\n", info.pid);
	printf("state: '%c'\n", info.state);
	printf("time: %lld.%.9ld sec\n", (long long)info.time.tv_sec, info.time.tv_nsec);
	printf("stack: %p\n", info.stack);
	printf("parent pid: %u\n", info.parent);
	printf("child(rens) pid(s): [ ");
	for (size_t i = 0; i < info.nb_children; i++) {
		printf("%u", info.children[i]);
		if (i < info.nb_children - 1)
			printf(", ");
	}
	printf(" ]\n");
	printf("exe: %s\n", info.exe);
	printf("root: %s\n", info.root);
	printf("pwd: %s\n", info.pwd);
	free(info.children);
	free(info.exe);
	free(info.root);
	free(info.pwd);
	return (0);
}
