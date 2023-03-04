KERNEL_PATH = /usr/src/kernel-5.18.0#$(shell uname -r)

all:
	mkdir -p $(KERNEL_PATH)/sys_get_pid_info/
	echo "obj-y := sys_get_pid_info.o" > $(KERNEL_PATH)/sys_get_pid_info/Makefile
	cp -f sys_get_pid_info.c $(KERNEL_PATH)/sys_get_pid_info/
	-patch -N $(KERNEL_PATH)/Makefile < Makefile.patch
	-patch -N $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl < syscall_64.tbl.patch
	make -C $(KERNEL_PATH)
	make -C $(KERNEL_PATH) modules_install install

clean:
	-patch -NR $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl < syscall_64.tbl.patch
	-patch -NR $(KERNEL_PATH)/Makefile < Makefile.patch
	rm -rf $(KERNEL_PATH)/sys_get_pid_info/
