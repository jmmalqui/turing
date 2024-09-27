#include "mem.h"

void* reallocate(void* pointer, size_t new_size)
{
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer, new_size);
    if (result == NULL)
        exit(1);
    return result;
}
