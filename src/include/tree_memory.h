#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

#define GROW_CAPACITY(number) ((number) < 8 ? 8 : (number) * 2)

#define GROW_ARRAY(type, ptr, new_count) \
    (type*)reallocate(ptr, sizeof(type) * (new_count))

#define FREE_ARRAY(type, ptr, old_count) \
    (type*)reallocate(ptr, sizeof(type)*old_count, 0)

void* reallocate(void* ptr, long long new_size) 
{
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    
    void* result = realloc(ptr, new_size);
    if (result == NULL) exit(1);
    return result;
}

#endif // MEMORY_H