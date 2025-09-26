//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#include "mem.h"

void* memset(void* dest, int val, size_t len) {
	uint8_t* ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}
