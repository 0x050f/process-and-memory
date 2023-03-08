#ifndef SYS_GET_PID_INFO_H
# define SYS_GET_PID_INFO_H

#include <limits.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

struct pid_info {
	pid_t				pid;
	uint8_t				state;
	void				*stack;
	struct timespec		time;
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children;
	size_t				children_len;
	char				*exe;
	char				*root;
	char				*pwd;
};

#endif
