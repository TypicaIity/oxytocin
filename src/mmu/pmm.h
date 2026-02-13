#pragma once

#define PAGE_NOACCESS	0
#define PAGE_READONLY	1
#define PAGE_WRITE		2
#define PAGE_EXECUTE	4

#define PAGE_EXECUTE_READ		PAGE_EXECUTE
#define PAGE_READWRITE			PAGE_WRITE
#define PAGE_EXECUTE_READWRITE	(PAGE_READWRITE | PAGE_EXECUTE)

void pmm_init();
void* allocpage(uint32_t prot);
void* allocpages(uint32_t npages, uint32_t prot);
void freepage(void* addr);
void freepages(void* addr, uint32_t npages);
uint32_t pmm_protect(void* addr, uint32_t prot);
uint32_t pmm_getProtection(void* addr);
size_t pmm_getFreePageCount();
size_t pmm_getUsedPageCount();
size_t pmm_getTotalPageCount();
