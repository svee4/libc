

#ifndef __h_vec
#define __h_vec

typedef struct {
	int length;
	int capacity;
	int item_size;
	void** items;
} vec;

vec* vec_new(int initial_capacity, int item_size);
void vec_create(vec* ptr, int initial_capacity, int item_size);
void vec_zero(vec* vec);
void vec_add(vec* ptr, void* value);

void vec_free(vec* ptr);
void vec_free_all(vec* ptr);

void vec_print(vec* vec, char* (*formatter)(void*, int));
char* vec_formatter_str(void* cur, int _);
char* vec_formatter_int(void* cur, int _);


#endif