#ifndef __h_list
#define __h_list

#include "lib.h"


struct list_entry {
	struct list_entry* prev;
	struct list_entry* next;
	void* value;
};

// typedef struct {
// 	struct list_entry* prev;
// 	struct list_entry* next;
// 	void* value;
// } list_entry;


typedef struct {
	int length;
	int item_size;
	struct list_entry* head;
	struct list_entry* tail;
} list;

list* list_new(int item_size);
void list_create(list* ptr, int item_size);
void list_free(list* ptr);
void list_free_all(list* ptr);

void list_append(list* ptr, void* value);
void list_prepend(list* ptr, void* value);
bool list_insert(list* ptr, int index, void* value);

struct list_entry* list_find(list* ptr, void* value);
int list_indexof(list* ptr, void* value);
struct list_entry* list_at(list* ptr, int index);

bool list_remove(list* ptr, void* value, bool free_value);
bool list_removeat(list* ptr, int index, bool free_value);

#endif
