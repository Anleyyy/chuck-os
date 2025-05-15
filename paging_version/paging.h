#define END_KERNEL 0x00400000
#define NB_PAGES_KERNEL 0x00000200
#define NB_PAGES 0x00080000
#define MEM_SIZE 0x80000000
#define PAGE_SIZE 0x00001000

typedef union {
    unsigned int raw;

    struct {
        unsigned int offset : 12;     // bits 0–11
        unsigned int table  : 10;     // bits 12–21
        unsigned int dir    : 10;     // bits 22–31
    } parts;

} virtual_address_t;

void init_pages();

void vmm_switch(unsigned int* new_page_directory);

void* phys_alloc_page();

unsigned int* vmm_create_pd(); 

void vmm_map(unsigned int* pd, virtual_address_t va, unsigned int flags);


void map_alias(unsigned int* page_directory, unsigned int virt1, unsigned int virt2, unsigned int flags);
