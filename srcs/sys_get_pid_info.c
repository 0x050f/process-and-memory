#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/syscalls.h>
#include <linux/sched.h>

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
	struct task_struct	*child;
	struct task_struct	*tsk;

	pid_info->pid = pid;
	tsk = find_task_by_vpid(pid);
	if (tsk) {
		printk("find_task_by_vpid: ok\n");
		pid_info->parent_pid = tsk->parent->pid;
		printk("parentpid: %u\n", tsk->parent->pid);
		list_for_each_entry(child, &tsk->children, sibling) {
			printk("childpid: %u\n", child->pid);
		}
		return (0);
	} else {
		printk("find_task_by_vpid: ko\n");
	}
	return (1);
}
