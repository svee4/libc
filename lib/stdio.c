#include "lib.h"
#include "mem.h"
#include "string.h"
#include "syscall.h"


/*
 *
 */
void print(char* str, int str_size) {
	syscall3(SYSCALL_WRITE, 1, (long)str, (long)str_size);
}

/*
 * print string with a trailing newline added
 */
void println(char* str, int str_size) {
	char new_str[str_size + 1];
	mem_copy(new_str, str, str_size);
	new_str[str_size] = '\n';
	new_str[str_size + 1] = '\0';
	print(new_str, str_size + 1);
}


/*
 * print string until first NULL character
 */
void printnl(char* str) {
	int len = str_size(str);
	print(str, len);
}

/*
 * print string until first NULL character, with a trailing newline
 */
void printnlln(char* str) {
	int len = str_size(str);
	println(str, len);
}

void printf(char* str) {}
