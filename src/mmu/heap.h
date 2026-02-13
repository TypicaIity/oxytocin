#pragma once

#define MIN_BLOCK_SIZE 32

typedef struct HeapEntry {
	uint64_t size : 63;
	uint64_t used : 1;
	struct HeapEntry* next;
} HeapEntry;

typedef struct {
	void* data;
	size_t totalBytes;
	HeapEntry* head;
	size_t freeBytes;
	size_t usedBytes;
} Heap;

Heap* heap_create(size_t size);
void heap_destroy(Heap* heap);

void heap_free(Heap* heap, void* addr);
void* heap_alloc(Heap* heap, size_t size);

size_t heap_getFreeBytes(Heap* heap);
size_t heap_getUsedBytes(Heap* heap);
size_t heap_getTotalBytes(Heap* heap);
