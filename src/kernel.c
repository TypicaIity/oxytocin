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

void kmain(BootInfo* info) {
	for (uint64_t i = 0; i < info->fb.height; i++) {
		for (uint64_t j = 0; j < info->fb.width; j++)
			info->fb.data[i * info->fb.pitch + j] = 0x00000000;
	}

	uint64_t startX = (info->fb.width - 200) / 2;
	uint64_t startY = (info->fb.height - 200) / 2;

	for (uint64_t y = startY; y < startY + 200; y++) {
		for (uint64_t x = startX; x < startX + 200; x++) {
			info->fb.data[y * info->fb.pitch + x] = 0x0000FF00;
		}
	}

	for (;;) {};
}
