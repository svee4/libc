
#ifndef __h_syscall
#define __h_syscall

typedef enum {
	SYSCALL_WRITE = 1,
	SYSCALL_MMAP = 9,
	SYSCALL_MUNMAP = 11,
	SYSCALL_NANOSLEEP = 35,
	SYSCALL_EXIT = 60
} syscall_num;

long syscall0(syscall_num num);
long syscall1(syscall_num num, long arg1);
long syscall2(syscall_num num, long arg1, long arg2);
long syscall3(syscall_num num, long arg1, long arg2, long arg3);
long syscall4(syscall_num num, long arg1, long arg2, long arg3, long arg4);
long syscall5(syscall_num num, long arg1, long arg2, long arg3, long arg4, long arg5);
long syscall6(syscall_num num, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

#endif
