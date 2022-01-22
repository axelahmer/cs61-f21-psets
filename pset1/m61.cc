#define M61_DISABLE 1
#include "m61.hh"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>
#include <iostream>



static m61_statistics gstats = {0, 0, 0, 0, 0, 0, UINTPTR_MAX, 0};

struct metadata {
    unsigned long long payload_size;
    bool freed;
    uintptr_t payload_ptr;

}; // sizeof(metadata) < header_size

const int header_size = 32; // bytes

/// m61_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then m61_malloc must
///    return a unique, newly-allocated pointer value. The allocation
///    request was at location `file`:`line`.

void* m61_malloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    
    metadata* meta_ptr = nullptr;
    void* payload_ptr;

    size_t total_sz = header_size + sz;

    // check for int overflow and malloc
    if (total_sz >= sz){
        meta_ptr = (metadata*)base_malloc(total_sz);
    }
    
    if (meta_ptr) // if not null pointer
    {
        payload_ptr = (void*)((char*)meta_ptr + header_size);

        // write to m61 stats struct
        ++gstats.ntotal;
        ++gstats.nactive;
        gstats.total_size+=sz;
        gstats.active_size+=sz;
        // update min and max heap values
        if((uintptr_t)meta_ptr < gstats.heap_min)
            gstats.heap_min = (uintptr_t)meta_ptr;
        if((uintptr_t)meta_ptr + total_sz > gstats.heap_max)
            gstats.heap_max = (uintptr_t)meta_ptr + total_sz;

        // write to pointer metadata
        meta_ptr->payload_size = sz;
        meta_ptr->freed = false;
        meta_ptr->payload_ptr = (uintptr_t)payload_ptr;
    }
    else
    {
        payload_ptr = nullptr;
        gstats.nfail++;
        gstats.fail_size+=sz;
    }
    
    return payload_ptr;
}


/// m61_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to m61_malloc. If `ptr == NULL`,
///    does nothing. The free was called at location `file`:`line`.

void m61_free(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // get pointer to metadata
    if (!ptr) return; // return if ptr = nullptr
    if ((uintptr_t)ptr < gstats.heap_min || (uintptr_t)ptr > gstats.heap_max){
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not in heap\n", file, line, ptr);
        abort();
        // MEMORY BUG: test024.cc:8: invalid free of pointer 0xffffffffffffffe0, not in heap
    }
    metadata* meta = (metadata*)((char*)ptr - header_size);
    if (meta->freed){
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, double free\n", file, line, ptr);
        abort();
        // MEMORY BUG???: invalid free of pointer ??ptr??, double free
    }
    if(meta->payload_ptr != (uintptr_t)ptr){
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n", file, line, ptr);
        abort();
    }
    if (meta)
    {
        // modify m61 stats
        --gstats.nactive;
        gstats.active_size -= meta->payload_size;
        meta->freed = true;
    }
    
    base_free(meta);
}


/// m61_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. If `sz == 0`,
///    then must return a unique, newly-allocated pointer value. Returned
///    memory should be initialized to zero. The allocation request was at
///    location `file`:`line`.

void* m61_calloc(size_t nmemb, size_t sz, const char* file, long line) {
    void* ptr;
    size_t array_size = nmemb * sz;
    if (nmemb != 0 && array_size / nmemb != sz) {
        // int overflow handling
        ptr = nullptr;
        gstats.nfail++;
        gstats.fail_size+=array_size;
    }
    else{
        ptr = m61_malloc(nmemb * sz, file, line);
        if (ptr) memset(ptr, 0, nmemb * sz);
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
