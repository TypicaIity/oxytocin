#pragma once

#include "types.h"

inline void* memset(void* dest, int val, size_t len) {
	uint8_t *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}
