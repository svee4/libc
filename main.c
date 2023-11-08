
#include "lib/_start.h"
#include "lib/stdio.h"
#include "lib/string.h"

typedef unsigned long ulong;

ulong fib(ulong n) {
	if (n <= 1) return n;
	return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[]) {
	printnlln(long_to_str(fib(12)));
	return 0;
}
