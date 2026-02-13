#pragma once

typedef uint64_t size_t;

typedef struct {
	void* mmap;
	uint64_t mmapSize;
	struct {
		uint32_t* data;
		uint32_t width;
		uint32_t height;
		uint32_t pitch;
	} fb;
} BootInfo;

#ifndef __cplusplus
#define nullptr 0
#define true 1
#define false 0
#define bool uint8_t
#endif

#define oxy_noret __attribute__((noreturn)) void
