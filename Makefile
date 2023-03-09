KERNEL_PATH	=	/usr/src/kernel-5.18.0#$(shell uname -r)

SRC			=	$(KERNEL_PATH)/sys_get_pid_info/sys_get_pid_info.c

all: $(SRC) $(KERNEL_PATH)/sys_get_pid_info/Makefile Makefile
	-patch -N $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl < patches/syscall_64.tbl.patch
	-patch -N $(KERNEL_PATH)/Makefile < patches/Makefile.patch
	make -C $(KERNEL_PATH)
	make -C $(KERNEL_PATH) modules_install install

$(SRC) $(KERNEL_PATH)/sys_get_pid_info/Makefile: | $(KERNEL_PATH)/sys_get_pid_info

$(KERNEL_PATH)/sys_get_pid_info:
	mkdir -p $(KERNEL_PATH)/sys_get_pid_info/

$(SRC):
	cp -f srcs/sys_get_pid_info.c $(KERNEL_PATH)/sys_get_pid_info/

$(KERNEL_PATH)/sys_get_pid_info/Makefile:
	echo "obj-y := sys_get_pid_info.o" > $(KERNEL_PATH)/sys_get_pid_info/Makefile

get_pid_info:
	gcc tests/get_pid_info.c -o get_pid_info

clean:
	-patch -NR $(KERNEL_PATH)/arch/x86/entry/syscalls/syscall_64.tbl < patches/syscall_64.tbl.patch
	-patch -NR $(KERNEL_PATH)/Makefile < patches/Makefile.patch
	rm -rf $(KERNEL_PATH)/sys_get_pid_info/
	rm -rf get_pid_info
