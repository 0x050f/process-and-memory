#ifndef SYS_GET_PID_INFO_H
# define SYS_GET_PID_INFO_H

#include <sys/time.h>
#include <unistd.h>
#include <stdint.h>

struct pid_info {
	pid_t				pid;
	uint8_t				state;
	void				*stack;
	struct timeval		time;
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children;
	size_t				children_len;
	char				*execpath;
	char				*fullpath;
};

#endif
