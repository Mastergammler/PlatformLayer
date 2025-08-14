/*
 * Stub file for kind of mapping out how the unix api works
 * Needs to be separated into map and linux at a later point,
 * because not everything is the equivalent
 */
#include <sys/mman.h>

void* platform_alloc(size_t size)
{
    void* mem = mmap(0,
                     size,
                     PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS,
                     -1,
                     0);
    return (mem == MAP_FAILED) ? NULL : mem;
}

void platform_free(void* mem, size_t size)
{
    munmap(mem, size);
}
