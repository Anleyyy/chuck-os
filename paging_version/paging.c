#include "paging.h"
#include "kmain.h"

void init_pages() {
    for (int i = 0; i < NB_PAGES_KERNEL; ++i)
        change_page(1, i);
    for (int i = NB_PAGES_KERNEL; i < NB_PAGES; ++i)
        change_page(0, i);
}

void vmm_switch(unsigned int* new_page_directory) {
    asm volatile ("mov %0, %%cr3" :: "r"(new_page_directory));
}

void* phys_alloc_page() {
    for (int i = NB_PAGES_KERNEL; i < NB_PAGES; ++i) {
        if (get_page(i) == 0) {
            change_page(1, i);
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

void vmm_map(unsigned int* pd, virtual_address_t va, unsigned int flags) {
    unsigned int new_page_table;
    if (!(pd[va.parts.dir] & 0x00000001)) {
        new_page_table = (unsigned int)phys_alloc_page();
        pd[va.parts.dir] = (new_page_table | flags);
    }
    unsigned int page_table = pd[va.parts.dir] & 0xFFFFF000;
    unsigned int* pt = (unsigned int*)page_table;
    unsigned int new_page;
    if (!(pt[va.parts.table] & 0x00000001)) {
        new_page = (unsigned int)phys_alloc_page();
        pt[va.parts.table] = (new_page | flags);
    }
}

void map_alias(unsigned int* page_directory, unsigned int virt1, unsigned int virt2, unsigned int flags) {
    virtual_address_t va1 = { .raw = virt1 };
    virtual_address_t va2 = { .raw = virt2 };
    
    // Récupérer le page table de virt1
    unsigned int* pt1 = (unsigned int*)(page_directory[va1.parts.dir] & 0xFFFFF000);
    unsigned int phys = pt1[va1.parts.table] & 0xFFFFF000;
    
    // Vérifie si le deuxième page table existe, sinon l'alloue
    if (!(page_directory[va2.parts.dir] & 0x1)) {
        unsigned int new_pt_phys = (unsigned int)phys_alloc_page(); // à définir ailleurs
        page_directory[va2.parts.dir] = new_pt_phys | flags;
    
        // Si tu n'as pas mappé l'espace 0x00000000 - 0x00400000,
        // tu dois mapper temporairement la page table pour y accéder ici.
        // (Ce code suppose que tu as identity-mappé les 4 premiers Mo.)
    }
    
    unsigned int* pt2 = (unsigned int*)(page_directory[va2.parts.dir] & 0xFFFFF000);
    pt2[va2.parts.table] = phys | flags;
    
    // Invalider l'entrée TLB de virt2
    asm volatile("invlpg (%0)" :: "r" ((void*)virt2) : "memory");
    
    }
