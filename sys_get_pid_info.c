#include <linux/kernel.h>
#include <linux/syscalls.h>

/*
#include <linux/pid>
#include <linux/unistd.h>
*/

/*
struct pid_info {
	pid_t			pid;
	uint8_t			state;
	void			*stack;
	struct timeval	time;
	pid_t			*child_pids;
	pid_t			parent_pid;
	char			*execpath;
	char			*fullpath;
}
*/

SYSCALL_DEFINE2(get_pid_info, void *, pid_info, int, pid)
{
	(void)pid_info;
	(void)pid;
	printk("Hello World\n");
	return (0);
}
