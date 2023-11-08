#include "sys.h"
#include "lib.h"
#include "syscall.h"

// https://github.com/torvalds/linux/blob/master/include/uapi/linux/time_types.h
typedef struct x {
	long long tv_sec;  /* seconds */
	long long tv_nsec; /* nanoseconds */
} __kernel_timespec;

void sleep(long ms) {
	const long MS2NS = 1000000;
	long s = 0;
	long ns = ms * MS2NS;

	if (ns > 999999999) {
		// too big
		s = ms / 1000;
		ns = (ms % 1000) * MS2NS;
	}
	__kernel_timespec t = (__kernel_timespec){
		.tv_sec = s,
		.tv_nsec = ns,
	};
	syscall2(SYSCALL_NANOSLEEP, (long)&t, NULL);
}
