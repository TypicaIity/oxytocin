#pragma once

#define isalpha(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define isalnum(c) (isalpha(c) || isdigit(c))

void* memset(void* dst, int val, size_t len);
void* memcpy(void* dst, const void* src, size_t len);

size_t strlen(const char* s);
bool streq(const char* s1, const char* s2);
