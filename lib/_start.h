
#ifndef __h_start
#define __h_start

#include "lib.h"
// startup code to be included once in main file

int main(int argc, char* argv[]);

int premain(int argc, char* argv) {
	char* args[argc];
	int i = 0, argv_cur_ptr = 0;
	for (i = 0; i < argc; i++) {
		args[i] = &(argv[argv_cur_ptr]);
		char* p = args[i];
		for (; *p != 0; p++) argv_cur_ptr++;
		argv_cur_ptr++;
	}
	return main(argc, args);
}

// https://stackoverflow.com/questions/2548486/compiling-without-libc
__attribute__((force_align_arg_pointer))  //
__attribute__((naked)) //
void _start() {
	asm("pushq %rbp;\n"
		"movq %rsp, %rbp;\n"
		"movq 8(%rsp), %rdi;\n"	  // argc
		"movq 16(%rsp), %rsi;\n"  // argv
		"call premain;\n"
		"mov %eax, %edi;\n"	 // rax is return value from main
		"mov $60, %eax;\n"	 // exit syscall number
		"syscall;\n"		 // exit
	);

	__builtin_unreachable();
}

#endif
