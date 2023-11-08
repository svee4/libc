#ifndef __h_hashmap
#define __h_hashmap

#include "list.h"
#include "vec.h"

typedef struct {
	char* key;
	void* value;
} hashmap_entry;

typedef struct {
	int size;
	int item_size;
	/*
	 * vec of list where value is list_entry value is hashmap_entry
	 */
	vec entries;
} hashmap;

#endif
