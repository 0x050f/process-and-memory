#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/syscalls.h>

struct pid_info {
	pid_t				pid;
	uint8_t				state;
	void				*stack;
	struct timespec64	time;
	pid_t				*child_pids;
	pid_t				parent_pid;
	char				*execpath;
	char				*fullpath;
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info *, pid_info, int, pid)
{
	(void)pid_info;
	(void)pid;
	printk("Hello World\n");
	return (0);
}
