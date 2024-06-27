#ifndef turing_common_h
#define turing_common_h

#include <ctype.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TM_ERROR 1
#define TM_SUCCESS 0

#define ALLOC(type) (type*)malloc(sizeof(type))
#define ALLOC_ARRAY(type, num) (type*)malloc((num) * sizeof(type))
#define ALLOC_STR(num) (char*)malloc((num) + 1)
#endif