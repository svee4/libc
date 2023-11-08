#include "vec.h"
#include "lib.h"
#include "mem.h"
#include "stdio.h"
#include "string.h"

static void ensure_capacity(vec* ptr, int needed) {
	const int increase_by = 1.5;

	if (ptr->capacity < ptr->length + needed) {
		if (ptr->capacity % 2 != 0) ptr->capacity++;
		int new_cap = ptr->capacity * increase_by;
		void* new_arr = malloc(new_cap * ptr->item_size);
		mem_copy(new_arr, ptr->items, ptr->length * ptr->item_size);
		free(ptr->items);

		ptr->items = new_arr;
		ptr->capacity = new_cap;
	}
}


vec* vec_new(int initial_capacity, int item_size) {
	vec* ptr = (vec*)malloc(sizeof(vec));
	ptr->length = 0;
	ptr->capacity = initial_capacity;
	ptr->item_size = item_size;
	void* arr = malloc(initial_capacity * item_size);
	ptr->items = arr;
	return ptr;
}

void vec_create(vec* ptr, int initial_capacity, int item_size) {
	ptr->length = 0;
	ptr->capacity = initial_capacity;
	ptr->item_size = item_size;
	void* arr = malloc(initial_capacity * item_size);
	ptr->items = arr;
}

/*
 * frees vec, but not the values that it holds
 */
void vec_free(vec* ptr) {
	free(ptr->items);
	free(ptr);
}

/*
 * frees vec, and all values that it references
 */
void vec_free_all(vec* ptr) {
	for (int i = 0; i < ptr->length; i++) {
		free(ptr->items[i]);
	}
	free(ptr->items);
	free(ptr);
}

/*
 * set all values in vec to 0
 */
void vec_zero(vec* vec) {
	mem_set(vec->items, 0, vec->length);
}

/*
 * add value to vec, increasing capacity if needed
 */
void vec_add(vec* ptr, void* value) {
	ensure_capacity(ptr, 1);
	mem_copy((void*)((long)(ptr->items) + (ptr->item_size * ptr->length)), &value, ptr->item_size);
	ptr->length++;
}

/*
 * formatter should return a malloced char*. the char* will be freed immediately after printed. the second argument is current index
 */
void vec_print(vec* vec, char* (*formatter)(void*, int)) {
	for (int i = 0; i < vec->length; i++) {
		char* item = formatter(vec->items[i], i);
		printnl(item);
		free(item);
	}
	println("", 1);
}

char* vec_formatter_str(void* cur, int _) {
	char* ptr = (char*)cur;
	int len = str_len(ptr);
	char* str2 = (char*)malloc(sizeof(char) * (len + 1));
	mem_copy(str2, ptr, len);
	str2[len] = ',';
	str2[len + 1] = ' ';
	str2[len + 2] = NULL;
	return str2;
}

char* vec_formatter_int(void* cur, int _) {
	char* ptr = long_to_str(*(int*)cur);
	int len = str_len(ptr);
	char* str2 = (char*)malloc(sizeof(char) * (len + 2));
	mem_copy(str2, ptr, len);
	free(ptr);
	str2[len] = ',';
	str2[len + 1] = ' ';
	str2[len + 2] = NULL;
	return str2;
}
