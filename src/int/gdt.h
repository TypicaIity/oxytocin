struct GDTEntry {
	uint16_t limitLow;
	uint16_t baseLow;
	uint8_t  baseMid;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  baseHigh;
} __attribute__((packed));

void gdt_init();
void gdt_setentry(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
