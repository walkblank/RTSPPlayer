
//#define STRINGCAT(x, y)  STRINGCAT_HELPER(x, y)
//#define STRINGCAT_HELPER(x, y)   x##y

#include <stdio.h>

#ifdef __cplusplus

//---------------------safe alloc/free---------------------------
extern unsigned int g_alloc_cnt;
extern unsigned int g_free_cnt;

void* safe_malloc(size_t size);
void* safe_realloc(void* oldptr, size_t newsize, size_t oldsize);
void* safe_calloc(size_t nmemb, size_t size);
void* safe_zalloc(size_t size);

#undef  SAFE_ALLOC
#define SAFE_ALLOC(ptr, size)\
    do {\
        void** pptr = (void**)&(ptr);\
        *pptr = safe_zalloc(size);\
        printf("alloc(%p, size=%lu)\tat [%s:%d:%s]\n", ptr, size, __FILE__, __LINE__, __FUNCTION__);\
    } while(0)

#define SAFE_ALLOC_SIZEOF(ptr)  SAFE_ALLOC(ptr, sizeof(*(ptr)))

#undef  SAFE_FREE
#define SAFE_FREE(ptr)\
    do {\
        if (ptr) {\
            printf("free( %p )\tat [%s:%d:%s]\n", ptr, __FILE__, __LINE__, __FUNCTION__);\
            free(ptr);\
            ptr = NULL;\
            ++g_free_cnt;\
        }\
    } while(0)

#endif
