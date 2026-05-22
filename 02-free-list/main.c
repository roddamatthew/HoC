#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "free-list.h"
#include "../include/utils.h"

int main()
{
    uint64_t ptr1 = (uint64_t)hmalloc(10);
    print_free_list(); // Initially should be empty
    
    // Make some more allocations
    uint64_t ptr2 = (uint64_t)hmalloc(20);
    uint64_t ptr3 = (uint64_t)hmalloc(50);

    // Inspect addresses of the new memory (note the 16 byte alignment)
    printf("ptr1: %p\n", (void *)ptr1);
    printf("ptr2: %p\n", (void *)ptr2);
    printf("ptr3: %p\n", (void *)ptr3);

    assert(ptr1 + 32 == ptr2);
    assert(ptr2 + 48 == ptr3);

    // Free the last chunk
    hfree((void *)ptr3);
    uint64_t ptr4 = (uint64_t)hmalloc(32);
    print_free_list();
    uint64_t ptr5 = (uint64_t)hmalloc(16);
    print_free_list();

    /* This allocation should pull from the free_list as there is an adequately large
    free chunk. It will reuse  */
    assert(ptr3 == ptr4);

    // This time we needed to use the top_chunk
    assert(ptr3 + 48 == ptr5);

    hfree((void *)ptr1);
    hfree((void *)ptr2);
    hfree((void *)ptr4);
    hfree((void *)ptr5);
    print_free_list();
}