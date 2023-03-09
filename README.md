# process-and-memory
Add a new syscall to your linux distribution `sys_get_pid_info`

## Installation

Install linux sources and edit Makefile's 'KERNEL_PATH' variable.
Then run `make` to copy source to the kernel path, then build and install the
 kernel. (If hopefully you are using named `vmlinuz` file to boot, otherwise you might
 have to update your grub config)

## Testing

The new syscall has the number `548`, you can use the header `includes/sys_get_pid_info.h` to use it.

```
#include "includes/sys_get_pid_info.h"

int		main(void) {
	struct pid_info info;

	bzero(&info, sizeof(struct pid_info));
	syscall(__NR_get_pid_info, &info, getpid());
	return (0);
}
```

Be careful with the struct `pid_info`, you must allocate space for the `children` array and fill `children_len` with the size of your array (in bytes). You must also allocate `exe`, `root` and `pwd` strings (with len capable of getting a MAX_PATH size).

You can also test the syscall by building up the test executable
```
make get_pid_info
```

Then launch it with a PID parameter
```
$ ./get_pid_info 1
Process Information:
pid: 1
state: 'S'
time: 4064.993140723 sec
stack: 0xffffa4cc40010000
parent pid: 0
child(rens) pid(s): [ 104, 118, 121, 163, 164, 165, 167, 170, 171, 173, 202 ]
exe: /usr/lib/systemd/systemd
root: /
pwd: /
```
