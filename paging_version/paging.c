#include "paging.h"

char pages[NB_PAGES];

void init_pages() {
    for (int i = 0; i < NB_PAGES_KERNEL; ++i)
        pages[i] = 1;
    for (int i = NB_PAGES_KERNEL; i < NB_PAGES; ++i)
        pages[i] = 0;
}

void vmm_switch(unsigned int* new_page_directory) {
    asm volatile ("mov %0, %%cr3" :: "r"(new_page_directory));
}

void* phys_alloc_page() {
    for (int i = NB_PAGES_KERNEL; i < NB_PAGES; ++i) {
        if (pages[i] == 0) {
            pages[i] = 1;
            unsigned int phys_addr = i * PAGE_SIZE;
            return (void*)phys_addr;
        };
        
    }
    unsigned int phys_addr = 0;
    return (void*)phys_addr;
}

unsigned int* vmm_create_pd() {
    unsigned int* new_page_directory = (unsigned int*)phys_alloc_page();
    return new_page_directory;
}

void vmm_map(unsigned int* pd, virtual_address_t va, unsigned int* pa, unsigned int flags) {
    unsigned int* a = pd;
    virtual_address_t b = va;
    unsigned int* c = pa;
    unsigned int d = flags;
    a=a; b=b; c=c; d=d;
}
