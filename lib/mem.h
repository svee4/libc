#ifndef __h_mem
#define __h_mem

#include "lib.h"


void* malloc(int size);

long free(void* ptr);

void mem_set(void* mem, char value, int length);

void* mem_copy(void* dest, void* source, int length);

bool mem_compare(void* first, void* second, int length);

#endif