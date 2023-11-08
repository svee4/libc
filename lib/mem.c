#include "mem.h"
#include "lib.h"
#include "syscall.h"

typedef struct {
	int len;
} mem_header;

/*
 * allocates length amout of bytes of heap memory and returns pointer to start of memory segment
 * memory is guaranteed to be zeroed
 */
void* malloc(int length) {

	// rax, rdi, rsi, rdx, r10, r8, r9
	// asm (
	// 	"mov $"STR(SYSCALL_MMAP)", %%rax;\n"  // syscall MMAP 9
	// 	"xor %%rdi, %%rdi;\n" // 0 = no preference for pointer location
	// 	"movq %[requested_size], %%rsi;\n" // size
	// 	"mov $7, %%rdx;\n" // protection level. 7 is for ALL (read, write, exec)
	// 	"mov $34, %%r10;\n" // flags. private | anonymous, idk what these do
	// 	"mov $-1, %%r8;\n" // file descriptor. -1 because we use anonymous
	// 	"xor %%r9, %%r9;\n" // offset of 0 into file. always for anonymous/private
	// 	"syscall;\n"
	// 	"mov %%rax, %[return_ptr];\n"
	// : [return_ptr]"=g"(ptr)
	// : [requested_size]"g"((long)length + sizeof(mem_header))
	// : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9");

	long ptr = syscall6(SYSCALL_MMAP,
						0,									// no preference for location
						(long)length + sizeof(mem_header),	// length to request
						7,									// protection level. 7 == READ | WRITE | EXEC
						34,									// flags. 34 == private and anonymous
						-1,									// file descriptor. -1 because we use anonymous
						0									// offset of 0 into file. because we use anonymous/private
	);

	mem_header head = (mem_header){.len = length};
	mem_copy((void*)ptr, &head, sizeof(mem_header));
	void* mem = (void*)(ptr + sizeof(mem_header));
	mem_set(mem, 0, length);
	return mem;
}

/*
 * free memory that was previously allocated with malloc
 */
long free(void* ptr) {
	ptr -= sizeof(mem_header);
	int len = ((mem_header*)ptr)->len;
	// asm("mov $" STR(SYSCALL_MUNMAP) ", %%rax;\n"			 // syscall MUNMAP 11
	// 								"mov %[addr], %%rdi;\n"	 // address of memory
	// 								"mov %[len], %%rsi;\n"	 // length of memory
	// 								"syscall;\n"
	// 	:
	// 	: [addr] "g"((long)ptr), [len] "g"((long)len)
	// 	: "rax", "rdi", "rsi");
	return syscall2(SYSCALL_MUNMAP, (long)ptr, (long)len + sizeof(mem_header));
}

/*
 * Set length bytes in mem to value
 */
void mem_set(void* mem, char value, int length) {
	for (int i = 0; i < length; i++) {
		*(char*)(mem + i) = value;
	}
}

/*
 * copy length bytes from source to dest
 */
void* mem_copy(void* dest, void* source, int length) {
	for (int i = 0; i < length; i++) {
		mem_set(dest + i, *(char*)(source + i), 1);
	}
	return dest;
}

/*
 * compare length amount of bytes starting from first and second. returns true if all bytes are equal
 */
bool mem_compare(void* first, void* second, int length) {
	byte* p1 = (byte*)first;
	byte* p2 = (byte*)second;

	for (int i = 0; i < length; i++) {
		if (*p1 != *p2) return false;
		p1++;
		p2++;
	}

	return true;
}
