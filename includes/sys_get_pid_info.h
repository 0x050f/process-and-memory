#ifndef SYS_GET_PID_INFO_H
# define SYS_GET_PID_INFO_H

# include <limits.h>
# include <stdint.h>
# include <sys/time.h>
# include <unistd.h>

# define __NR_get_pid_info 548

/*
 * State in 'RSDTtXZPI':
 * 'R': running
 * 'S': sleeping
 * 'D': disk sleep
 * 'T': stopped
 * 't': tracing stop
 * 'X': dead
 * 'Z': zombie
 * 'P': parked
 * 'I': idle
*/

struct pid_info {
	pid_t				pid;
	char				state;
	struct timespec		time;
	void				*stack;
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children; // array
	size_t				children_len;
	char				*exe;
	char				*root;
	char				*pwd;
};

#endif
