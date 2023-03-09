#include <linux/errno.h>
#include <linux/fs_struct.h>
#include <linux/kernel.h>
#include <linux/limits.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/syscalls.h>
#include <linux/time.h>

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
	struct timespec64	time;
	void				*stack;
	pid_t				parent;
	size_t				nb_children;
	pid_t				*children; // array
	size_t				children_len;
	char				*exe;
	char				*root;
	char				*pwd;
};

int				get_pwd(char * __user dst, size_t len, struct task_struct *tsk)
{
	int					ret;
	struct path			pwd;
	char				*path;
	char				*buffer;

	ret = 0;
	buffer = kmalloc(len, GFP_KERNEL);
	if (!buffer)
		return (-ENOMEM);
	get_fs_pwd(tsk->fs, &pwd);
	path = d_path(&pwd, buffer, len);
	if (len < strlen(path))
		ret = -ENOMEM;
	if (!ret && copy_to_user(dst, path, strlen(path)))
		ret = -EFAULT;
	kfree(buffer);
	return (ret);
}

int				get_root(char * __user dst, size_t len, struct task_struct *tsk)
{
	int					ret;
	struct path			root;
	char				*path;
	char				*buffer;

	ret = 0;
	buffer = kmalloc(len, GFP_KERNEL);
	if (!buffer)
		return (-ENOMEM);
	get_fs_root(tsk->fs, &root);
	path = d_path(&root, buffer, len);
	if (len < strlen(path))
		ret = -ENOMEM;
	if (!ret && copy_to_user(dst, path, strlen(path)))
		ret = -EFAULT;
	kfree(buffer);
	return (ret);
}

int				get_exe(char * __user dst, size_t len, struct task_struct *tsk)
{
	int					ret;
	char				*buffer;
	char				*path;
	struct file			*exe_file;

	ret = 0;
	buffer = kmalloc(len, GFP_KERNEL);
	if (!buffer)
		return (-ENOMEM);
	exe_file = get_task_exe_file(tsk);
	if (!exe_file) {
		if (copy_to_user(dst, "[", 1) ||
			copy_to_user(dst + 1, tsk->comm, strlen(tsk->comm)) ||
			copy_to_user(dst + strlen(tsk->comm) + 1, "]", 1))
			ret = -EFAULT;
		kfree(buffer);
		return (ret);
	}
	path_get(&exe_file->f_path);
	path = file_path(exe_file, buffer, len);
	if (len < strlen(path))
		ret = -ENOMEM;
	if (!ret && copy_to_user(dst, path, strlen(path)))
		ret = -EFAULT;
	kfree(buffer);
	return (ret);
}

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, pid_info, int, pid)
{
	int					ret;
	struct pid_info		kpid_info;
	struct task_struct	*child;
	struct task_struct	*tsk;
	pid_t				*tmp;

	if (copy_from_user(&kpid_info, pid_info, sizeof(struct pid_info)))
		return (-EFAULT);
	kpid_info.pid = pid;
	tsk = find_task_by_vpid(pid);
	if (tsk) {
		kpid_info.parent = tsk->parent->pid;
		kpid_info.state = task_state_to_char(tsk);
		kpid_info.time = ns_to_timespec64((ktime_get_ns() - tsk->start_time));
		kpid_info.stack = tsk->stack;
		kpid_info.nb_children = 0;
		list_for_each_entry(child, &tsk->children, sibling) {
			kpid_info.nb_children++;
		}
		if (kpid_info.nb_children * sizeof(pid_t) > kpid_info.children_len)
			return (-ENOMEM);
		tmp = kpid_info.children;
		list_for_each_entry(child, &tsk->children, sibling) {
			*tmp = child->pid;
			if (copy_to_user(tmp, &child->pid, sizeof(pid_t)))
				return (-EFAULT);
			tmp += 1;
		}
		if (copy_to_user(pid_info, &kpid_info, sizeof(struct pid_info) - (sizeof(char *) * 3))) {
			kfree(kpid_info.exe);
			return (-EFAULT);
		}
		// exe - root - pwd
		if (pid_info->exe) {
			ret = get_exe(pid_info->exe, PATH_MAX, tsk);
			if (ret)
				return (ret);
		}
		if (pid_info->root) {
			ret = get_root(pid_info->root, PATH_MAX, tsk);
			if (ret)
				return (ret);
		}
		if (pid_info->pwd) {
			ret = get_pwd(pid_info->pwd, PATH_MAX, tsk);
			if (ret)
				return (ret);
		}
		return (0);
	}
	return (-ESRCH);
}
