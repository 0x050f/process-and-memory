#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/errno.h>

struct pid_info {
	pid_t				pid;
	uint8_t				state; // TODO
	void				*stack; // TODO
	struct timespec64	time; // TODO
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children;
	size_t				children_len;
	char				*execpath; // TODO
	char				*fullpath; // TODO
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, pid_info, int, pid)
{
	struct pid_info		kpid_info;
	struct task_struct	*child;
	struct task_struct	*tsk;
	pid_t				*tmp;

	kpid_info.pid = pid;
	kpid_info.children = pid_info->children;
	kpid_info.children_len = pid_info->children_len;
	tsk = find_task_by_vpid(pid);
	if (tsk) {
		kpid_info.parent = tsk->parent->pid;
		kpid_info.nb_children = 0;
		list_for_each_entry(child, &tsk->children, sibling) {
			kpid_info.nb_children++;
		}
		if (kpid_info.nb_children * sizeof(pid_t) > kpid_info.children_len)
			return (-ENOMEM);
		pid_t *tmp = kpid_info.children;
		list_for_each_entry(child, &tsk->children, sibling) {
			*tmp = child->pid;
			if (copy_to_user(tmp, &child->pid, sizeof(pid_t)))
				return (-EFAULT);
			tmp += 1;
		}
		if (copy_to_user(pid_info, &kpid_info, sizeof(struct pid_info)))
			return (-EFAULT);
		return (0);
	}
	return (-ESRCH);
}
