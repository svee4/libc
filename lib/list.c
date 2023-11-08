#include "list.h"
#include "lib.h"
#include "mem.h"



/*
 * allocate new list
 */
list* list_new(int item_size) {
	list* p = (list*)malloc(sizeof(list));
	list_create(p, item_size);
	return p;
}

void list_create(list* ptr, int item_size) {
	mem_set(ptr, 0, sizeof(list));
	ptr->item_size = item_size;
}

/*
 * free resources used by this list. does not free the values themselves
 */
void list_free(list* ptr) {
	struct list_entry* cur = ptr->head;
	while (cur) {
		struct list_entry* next = cur->next;
		free(cur);
		cur = next;
	}
	free(ptr);
}

/*
 * free resources used by this list, as well as values that its entries point to
 */
void list_free_all(list* ptr) {
	struct list_entry* cur = ptr->head;
	while (cur) {
		struct list_entry* next = cur->next;
		free(cur->value);
		free(cur);
		cur = next;
	}
	free(ptr);
}

static struct list_entry* new_entry(void* value, struct list_entry* prev, struct list_entry* next) {
	struct list_entry* p = (struct list_entry*)malloc(sizeof(struct list_entry));
	p->value = (struct list_entry*)value;
	p->prev = prev;
	p->next = next;
	return p;
}

/*
 * add value to the end of this list
 */
void list_append(list* ptr, void* value) {
	struct list_entry* v = new_entry(value, NULL, NULL);

	if (ptr->tail) {
		v->prev = (struct list_entry*)ptr->tail;
		ptr->tail->next = (struct list_entry*)v;
	} else {
		ptr->head = v;
	}
	ptr->tail = v;
	ptr->length++;
}

/*
 * add value to the beginning of this list
 */
void list_prepend(list* ptr, void* value) {
	struct list_entry* v = new_entry(value, NULL, NULL);
	if (ptr->head) {
		v->next = (struct list_entry*)ptr->head;
		ptr->head->prev = (struct list_entry*)v;
	}
	ptr->head = v;
	if (!ptr->tail) ptr->tail = v;
	ptr->length++;
}

static void insert_before(struct list_entry* beforethis, struct list_entry* item) {
	if (beforethis->prev) {
		beforethis->prev->next = (struct list_entry*)item;
		item->prev = (struct list_entry*)beforethis->prev;
	}
	beforethis->prev = (struct list_entry*)item;
	item->next = (struct list_entry*)beforethis;
}

/*
 * insert a value to given index in this list
 * returns false if index was bigger than length, or less than 0
 */
bool list_insert(list* ptr, int index, void* value) {
	if (index > ptr->length) return false;
	if (index == ptr->length) {
		list_append(ptr, value);
		return true;
	}
	if (index == 0) {
		list_prepend(ptr, value);
		return true;
	}

	struct list_entry* cur = ptr->head;
	int i = 0;
	while (cur) {
		if (i == index) {
			insert_before(cur, new_entry(value, NULL, NULL));
			ptr->length++;
			return true;
		}
		cur = (struct list_entry*)cur->next;
		i++;
	}
	return false;
}

/*
 * find entry in list by comparing the value of each entry to given value until it is equal.
 * returns the first list_entry with an equal value, or NULL if none match
 */
struct list_entry* list_find(list* ptr, void* value) {
	struct list_entry* cur = ptr->head;
	while (cur) {
		if (mem_compare(cur->value, value, ptr->item_size)) return cur;
		cur = (struct list_entry*)cur->next;
	}
	return NULL;
}

/*
 * find index of given value by comparing list values until a match is found.
 * returns index of entry with matching value, or NULL if none found
 */
int list_indexof(list* ptr, void* value) {
	const int NOTFOUND = -1;
	int index = 0;
	struct list_entry* cur = ptr->head;
	while (cur) {
		if (mem_compare(cur->value, value, ptr->item_size)) return index;
		cur = (struct list_entry*)cur->next;
		index++;
	}
	return NOTFOUND;
}

/*
 * returns the item in list at given index, or NULL if index is not a valid index to list
 */
struct list_entry* list_at(list* ptr, int index) {
	struct list_entry* cur = ptr->head;
	int i = 0;
	while (cur) {
		if (i == index) return cur;
		cur = (struct list_entry*)cur->next;
		i++;
	}
	return NULL;
}

static void remove_entry(struct list_entry* ptr) {
	ptr->prev->next = ptr->next;
	ptr->next->prev = ptr->prev;
	free(ptr);
}

/*
 * Remove entry from list by value. compares all values in list until a match is found, and then removes that match.
 * If free_value is true, the value that the found entry points to will also be freed. Otherwise the value will be left possibly dangling.
 * Returns false if no values matched
 */
bool list_remove(list* ptr, void* value, bool free_value) {
	struct list_entry* cur = ptr->head;
	while (cur) {
		if (mem_compare(cur->value, value, ptr->item_size)) {
			if (free_value) free(cur->value);
			if (cur == ptr->head) ptr->head = (struct list_entry*)cur->next;
			if (cur == ptr->tail) ptr->tail = (struct list_entry*)cur->prev;
			remove_entry(cur);
			ptr->length--;
			return true;
		}
		cur = (struct list_entry*)cur->next;
	}
	return false;
}

/*
 * Removes entry from list at given index.
 * If free_value is true, the value that the entry points to is also freed. Otherwise, be careful to not leave the value dangling.
 * Returns false if index is not a valid index to list
 */
bool list_removeat(list* ptr, int index, bool free_value) {
	struct list_entry* cur = ptr->head;
	int i = 0;
	while (cur) {
		if (i == index) {
			if (free_value) free(cur->value);
			remove_entry(cur);
			ptr->length--;
			return true;
		} else {
			cur = (struct list_entry*)cur->next;
			i++;
		}
	}
	return false;
}
