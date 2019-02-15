#include "no27/common/memory.h"

#include <stdlib.h>
#include <stdio.h>

void *
no27_malloc(size_t size) {
    void *memory = malloc(size);
    if(memory == NULL) {
        fprintf(stderr, "error: no memory. aborting.\n");
        abort();
    }

    return memory;
}

void
no27_free(void *memory) {
    free(memory);
}