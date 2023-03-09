#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "../includes/sys_get_pid_info.h"

int		main(void) {
	uint8_t buffer[200];
	char root[4096];
	char exe[4096];
	char pwd[4096];
	struct pid_info info;

	info.children = (void *)buffer;
	info.children_len = 200;
	info.root = (char *)root;
	info.exe = (char *)exe;
	info.pwd = (char *)pwd;
	if (fork()) {
		int ret = syscall(__NR_get_pid_info, &info, getpid());
		printf("ret: %d\n", ret);
		if (ret < 0) {
			dprintf(STDERR_FILENO, "errno: %s\n", strerror(errno));
			return (ret);
		}
		printf("pid: %u\n", info.pid);
		printf("state: '%c'\n", info.state);
		printf("time: %lld.%.9ld sec\n", (long long)info.time.tv_sec, info.time.tv_nsec);
		printf("parent pid: %u\n", info.parent);
		for (size_t i = 0; i < info.nb_children; i++)
			printf("child[%zu] pid: %u\n", i, info.children[i]);
		printf("exe: %s\n", info.exe);
		printf("root: %s\n", info.root);
		printf("pwd: %s\n", info.pwd);
		printf("stack: %p\n", info.stack);
	} else {
		sleep(10);
	}
	return (0);
}
