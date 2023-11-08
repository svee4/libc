#include "hashmap.h"
#include "lib.h"
#include "list.h"
#include "mem.h"
#include "string.h"
#include "vec.h"

static const int MAX_LIST_LENGTH = 4;

static int hash(char* key, int map_size) {
	int h = 0;
	// https://stackoverflow.com/a/69812981
	// One-byte-at-a-time hash based on Murmur's mix
	// Source: https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp
	for (; *key; ++key) {
		h ^= *key;
		h *= 0x5bd1e995;
		h ^= h >> 15;
	}
	return h % map_size;
}

static hashmap_entry* hashmap_entry_new(char* key, void* value) {
	hashmap_entry* ptr = (hashmap_entry*)malloc(sizeof(hashmap_entry));
	int key_size = str_size(key);
	char* key2 = (char*)malloc(sizeof(char) * key_size);
	mem_copy(key2, key, key_size);
	ptr->key = key2;
	ptr->value = value;
	return ptr;
}

static void resize_hashmap(hashmap* ptr, int new_size) {

	int prev_length = ptr->size;
	list* lists[prev_length];

	for (int i = 0; i < prev_length; i++) {
		list* list = ptr->entries.items[i];
		list_create(lists[i], sizeof(hashmap_entry));
		struct list_entry* cur = list->head;
		while (cur) {
			hashmap_entry* v = (hashmap_entry*)cur->value;
			char* key = (char*)malloc(sizeof(char) * str_size(v->key));
			list_append(lists[i], hashmap_entry_new(key, v->value));
		}
	}
}


hashmap* hashmap_new(int item_size) {
	const int INITIAL_SIZE = 5;
	hashmap* ptr = (hashmap*)malloc(sizeof(hashmap));
	ptr->size = INITIAL_SIZE;
	vec_create(&ptr->entries, INITIAL_SIZE, sizeof(list));
	return ptr;
}

void hashmap_free(hashmap* ptr, bool free_value) {
	for (int i = 0; i < ptr->size; i++) {
		list* list = ptr->entries.items[i];
		if (!list) continue;

		struct list_entry* cur = list->head;
		while (cur) {
			free(((hashmap_entry*)cur->value)->key);
			if (free_value) free(((hashmap_entry*)cur->value)->value);
			struct list_entry* next = (struct list_entry*)cur->next;
			free(cur);
			cur = next;
		}
		free(list);
	}
	free(ptr->entries.items);
}



void* hashmap_get(hashmap* ptr, char* key) {
	if (ptr->entries.length == 0) return NULL;
	int h = hash(key, ptr->size);

	list* list = ptr->entries.items[h];
	if (!list || list->length == 0) return NULL;
	if (list->length == 1) return ((hashmap_entry*)list->head->value)->value;

	struct list_entry* cur = list->head;
	while (cur) {
		hashmap_entry* entry = (hashmap_entry*)cur->value;
		if (str_equals(key, entry->key)) return entry->value;
		cur = (struct list_entry*)cur->next;
	}
	return NULL;
}



void hashmap_add(hashmap* ptr, char* key, void* value) {
	int h = hash(value, ptr->size);
	list* list = ptr->entries.items[h];
	if (!list) {
		list = list_new(sizeof(hashmap_entry));
		ptr->entries.items[h] = list;
	}
	list_append(list, hashmap_entry_new(key, value));
	if (list->length > MAX_LIST_LENGTH) {
		resize_hashmap(ptr, list->length * 1.5);
	}
}

static void free_entry(hashmap_entry* ptr, bool free_value) {
	free(ptr->key);
	if (free_value) free(ptr->value);
	free(ptr);
}

bool hashmap_remove(hashmap* ptr, char* key, bool free_value) {
	int h = hash(key, ptr->size);
	list* list = ptr->entries.items[h];
	if (!list) return false;

	struct list_entry* cur = list->head;
	if (list->length == 1) {
		hashmap_entry* entry = (hashmap_entry*)cur->value;
		list_removeat(list, 0, false);
		free_entry(entry, free_value);
		free(list);
		ptr->entries.items[h] = NULL;
		return true;
	}

	while (cur) {
		if (str_equals(key, ((hashmap_entry*)cur->value)->key)) {
			hashmap_entry* entry = (hashmap_entry*)cur->value;
			list_remove(list, cur->value, false);
			free_entry(entry, free_value);
			return true;
		}
		cur = (struct list_entry*)cur->next;
	}
	return false;
}
