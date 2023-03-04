#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/errno.h>

struct pid_info {
	pid_t				pid;
	uint8_t				state;
	void				*stack;
	struct timespec64	time;
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children;
	size_t				children_len;
	char				*execpath;
	char				*fullpath;
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info *, pid_info, int, pid)
{
	struct task_struct	*child;
	struct task_struct	*tsk;
	pid_t				*tmp;

	pid_info->pid = pid;
	tsk = find_task_by_vpid(pid);
	if (tsk) {
		pid_info->parent = tsk->parent->pid;
		pid_info->nb_children = 0;
		list_for_each_entry(child, &tsk->children, sibling) {
			pid_info->nb_children++;
		}
		if (pid_info->nb_children * sizeof(pid_t) > pid_info->children_len)
			return (-ENOMEM);
		pid_t *tmp = pid_info->children;
		list_for_each_entry(child, &tsk->children, sibling) {
			*tmp = child->pid;
			tmp += 1;
		}
		return (0);
	}
	return (-ESRCH);
}
