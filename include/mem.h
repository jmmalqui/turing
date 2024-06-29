#ifndef turing_mem_h
#define turing_mem_h

#include "common.h"

#define ALLOC(type) (type *)malloc(sizeof(type))
#define ALLOC_ARRAY(type, num) (type *)malloc((num) * sizeof(type))
#define ALLOC_STR(num) (char *)malloc((num) + 1)
#define GROW_ARRAY(type, pointer, new_size) (type *)reallocate(pointer, sizeof(type) * (new_size))
#define FREE_ARRAY(type, pointer) (type *)reallocate(pointer, 0)
#define ARRAY_GROWTH 2

void *reallocate(void *pointer, size_t new_size);

#endif