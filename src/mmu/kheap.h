#pragma once

void kheap_init(size_t size);

void* kmalloc(size_t size);
void* kcalloc(size_t size, size_t n);
void* krealloc(void* ptr, size_t size);

void kfree(void* ptr);
