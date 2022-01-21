#define M61_DISABLE 1
#include "m61.hh"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>
#include <iostream>

static m61_statistics gstats = {0, 0, 0, 0, 0, 0, 0, 0};

/// m61_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc must
///    return a unique, newly-allocated pointer value. The allocation
///    request was at location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings

    // call malloc
    void* ret_ptr = base_malloc(sz);

    // check if first allocation and set heap min
    if(gstats.heap_min == 0) gstats.heap_min = (uintptr_t)ret_ptr;

    
    // std::cout << (int*)ret_ptr << std::endl;
    if (ret_ptr) // if not null pointer
    {
        ++gstats.ntotal;
        ++gstats.nactive;
        gstats.total_size+=sz;
        gstats.active_size+=sz;

        // update min and max heap values
        if((uintptr_t)ret_ptr < gstats.heap_min) gstats.heap_min = (uintptr_t)ret_ptr;
        if((uintptr_t)ret_ptr + sz > gstats.heap_max) gstats.heap_max = (uintptr_t)ret_ptr + sz;
    }
    else
    {
        gstats.nfail++;
        gstats.fail_size+=sz;
    }


    // if ((uintptr_t)ret_ptr < gstats.heap_min)
    // {
    //     gstats.heap_min = ret_ptr;
    // }
    // if ((uintptr_t)ret_ptr > gstats.heap_max)
    // {
    //     gstats.heap_max = ret_ptr;
    // }

    return ret_ptr;
}


/// m61_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to m61_malloc. If `ptr == NULL`,
///    does nothing. The free was called at location `file`:`line`.

void m61_free(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    if (ptr)
    {
        --gstats.nactive;
    }
    
    base_free(ptr);
}


/// m61_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. If `sz == 0`,
///    then must return a unique, newly-allocated pointer value. Returned
///    memory should be initialized to zero. The allocation request was at
///    location `file`:`line`.

void* m61_calloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test019).
    void* ptr = m61_malloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}


/// m61_get_statistics(stats)
///    Store the current memory statistics in `*stats`.

void m61_get_statistics(m61_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    memcpy(stats, &gstats, sizeof(gstats));
}


/// m61_print_statistics()
///    Print the current memory statistics.

void m61_print_statistics() {
    m61_statistics stats;
    m61_get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}


/// m61_print_leak_report()
///    Print a report of all currently-active allocated blocks of dynamic
///    memory.

void m61_print_leak_report() {
    // Your code here.
}


/// m61_print_heavy_hitter_report()
///    Print a report of heavily-used allocation locations.

void m61_print_heavy_hitter_report() {
    // Your heavy-hitters code here
}
