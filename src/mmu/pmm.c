#include "pmm.h"

#define PAGE_SIZE 4096

#define EfiConventionalMemory 7

typedef struct {
	uint32_t type;
	uint64_t physicalStart;
	uint64_t virtualStart;
	uint64_t numberOfPages;
	uint64_t attribute;
} EFI_MEMORY_DESCRIPTOR;

extern BootInfo* gInfo;

static uint8_t* protMap;
static uint32_t* bitmap;
static size_t totalPages, usedPages;
static uintptr_t memStart, memEnd;

#define isFree(idx) !(bitmap[(idx) / 32] & (1 << ((idx) & 32)))
#define markUsed(idx) bitmap[(idx) / 32] |= (1u << ((idx) & 31))
#define markFree(idx) bitmap[(idx) / 32] &= ~(1u << ((idx) & 31))
#define addrtopage(addr) ((uintptr_t)addr - memStart) / PAGE_SIZE
#define pagetoaddr(page) (void*)(memStart + page * PAGE_SIZE)

static size_t findpage(size_t npages) {
	size_t consecutive = 0;
	size_t startPage = 0;
	
	for (size_t i = 0; i < totalPages; i++) {
		if (!isFree(i)) {
			if (consecutive == 0)
				startPage = i;
			consecutive++;
			if (consecutive == npages)
				return startPage;
		} else
			consecutive = 0;
	}
	
	return (size_t)-1;
}

void pmm_init() {
	if (!gInfo || !gInfo->mmap)
		return;
	
	EFI_MEMORY_DESCRIPTOR* mmap = (EFI_MEMORY_DESCRIPTOR*)gInfo->mmap;
	size_t nentries = gInfo->mmapSize / sizeof(EFI_MEMORY_DESCRIPTOR);
	
	memStart = UINTPTR_MAX, memEnd = 0;
	memEnd = 0;
	
	for (size_t i = 0; i < nentries; i++) {
		if (mmap[i].type == EfiConventionalMemory) {
			uintptr_t start = (uintptr_t)mmap[i].physicalStart;
			uintptr_t end = start + (mmap[i].numberOfPages * PAGE_SIZE);
			
			if (start < memStart) memStart = start;
			if (end > memEnd) memEnd = end;
		}
	}
	
	totalPages = (memEnd - memStart) / PAGE_SIZE;
	usedPages = 0;
	
	size_t bitmapSize = (totalPages + 32 - 1) / 32;
	bitmap = (uint32_t*)memStart;
	protMap = (uint8_t*)(memStart + bitmapSize * sizeof(uint32_t));
	
	for (size_t i = 0; i < bitmapSize; i++)
		bitmap[i] = 0;
	
	for (size_t i = 0; i < totalPages; i++)
		protMap[i] = PAGE_NOACCESS;
	
	for (size_t i = 0; i < totalPages; i++)
		markUsed(i);
	
	for (size_t i = 0; i < nentries; i++) {
		if (mmap[i].type == EfiConventionalMemory) {
			uintptr_t start = (uintptr_t)mmap[i].physicalStart;
			size_t pageCount = mmap[i].numberOfPages;
			size_t startPage = (start - memStart) / PAGE_SIZE;
			
			for (size_t j = 0; j < pageCount; j++) {
				if (startPage + j < totalPages)
					markFree(startPage + j);
			}
		}
	}
	
	size_t metadataPages = ((bitmapSize * sizeof(uint32_t) + totalPages + PAGE_SIZE - 1) / PAGE_SIZE);
	for (size_t i = 0; i < metadataPages; i++) {
		markUsed(i);
		usedPages++;
	}
}

void* allocpage(uint32_t prot) {
	return allocpages(1, prot);
}

void* allocpages(uint32_t npages, uint32_t prot) {
	if (npages == 0)
		return nullptr;
	
	size_t startPage = findpage(npages);
	if (startPage == (size_t)-1)
		return nullptr;
	
	for (size_t i = 0; i < npages; i++) {
		markUsed(startPage + i);
		protMap[startPage + i] = (uint8_t)prot;
		usedPages++;
	}
	
	void* addr = pagetoaddr(startPage);
	
	uint8_t* ptr = (uint8_t*)addr;
	for (size_t i = 0; i < npages * PAGE_SIZE; i++)
		ptr[i] = 0;
	
	return addr;
}

void freepage(void* addr) {
	freepages(addr, 1);
}

void freepages(void* addr, uint32_t npages) {
	if (!addr || npages == 0)
		return;
	
	size_t startPage = addrtopage(addr);
	
	if (startPage >= totalPages || startPage + npages > totalPages)
		return;
	
	for (size_t i = 0; i < npages; i++) {
		if (isFree(startPage + i)) {
			markFree(startPage + i);
			protMap[startPage + i] = PAGE_NOACCESS;
			usedPages--;
		}
	}
}

uint32_t pmm_protect(void* addr, uint32_t prot) {
	if (!addr)
	return 0;
	
	size_t page = addrtopage(addr);
	
	if (page >= totalPages)
		return 0;
	
	if (!isFree(page))
		return 0;
	
	uint32_t oldProt = protMap[page];
	protMap[page] = (uint8_t)prot;
	return oldProt;
}

uint32_t pmm_getProtection(void* addr) {
	if (!addr)
		return PAGE_NOACCESS;
	
	size_t page = addrtopage(addr);
	
	if (page >= totalPages)
		return PAGE_NOACCESS;
	
	return protMap[page];
}

size_t pmm_getFreePageCount() {
	return totalPages - usedPages;
}

size_t pmm_getUsedPageCount() {
	return usedPages;
}

size_t pmm_getTotalPageCount() {
	return totalPages;
}