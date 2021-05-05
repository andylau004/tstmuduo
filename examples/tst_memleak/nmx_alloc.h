

#pragma once



#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <stdint.h>



void *nmx_alloc(size_t size);
void *nmx_calloc(size_t size);
void *nmx_realloc(void *p, size_t size);

#define nmx_free          free

#define nmx_align_ptr(p, a) \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))
    // (unsigned char *) (((unsigned int ) (p) + ((unsigned int ) a - 1)) & ~((unsigned int ) a - 1))

void *nmx_memalign(size_t alignment, size_t size);

