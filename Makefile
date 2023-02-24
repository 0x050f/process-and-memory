KERNEL_PATH = /usr/src/kernel-5.18.0#$(shell uname -r)

all:
	mkdir -p $(KERNEL_PATH)/sys_get_pid_info/
	echo "obj-y := sys_get_pid_info.o" > $(KERNEL_PATH)/sys_get_pid_info/Makefile
	cp -f sys_get_pid_info.c $(KERNEL_PATH)/sys_get_pid_info/
	(grep -E 'core-y\s+\+=' $(KERNEL_PATH)/Makefile | grep 'sys_get_pid_info/') || sed -i -E '/core-y\s+\+=/ s/$$/ sys_get_pid_info\//' $(KERNEL_PATH)/Makefile
	(grep sys_get_pid_info $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl) || echo "548	64	get_pid_info		sys_get_pid_info" >> $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl
	(grep sys_get_pid_info $(KERNEL_PATH)/include/linux/syscalls.h) || sed -i '$$ i\asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid);' $(KERNEL_PATH)/include/linux/syscalls.h
	make -C $(KERNEL_PATH)
	make -C $(KERNEL_PATH) modules_install install
