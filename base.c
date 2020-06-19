
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned int g_alloc_cnt = 0;
unsigned int g_free_cnt = 0;

void* safe_malloc(size_t size) {
    ++g_alloc_cnt;
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "malloc failed!\n");
        exit(-1);
    }
    return ptr;
}

void* safe_realloc(void* oldptr, size_t newsize, size_t oldsize) {
    ++g_alloc_cnt;
    ++g_free_cnt;
    void* ptr = realloc(oldptr, newsize);
    if (!ptr) {
        fprintf(stderr, "realloc failed!\n");
        exit(-1);
    }
    if (newsize > oldsize) {
        memset((char*)ptr + oldsize, 0, newsize - oldsize);
    }
    return ptr;
}

void* safe_calloc(size_t nmemb, size_t size) {
    ++g_alloc_cnt;
    void* ptr =  calloc(nmemb, size);
    if (!ptr) {
        fprintf(stderr, "calloc failed!\n");
        exit(-1);
    }
    return ptr;
}

void* safe_zalloc(size_t size) {
    ++g_alloc_cnt;
    void* ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "malloc failed!\n");
        exit(-1);
    }
    memset(ptr, 0, size);
    return ptr;
}
