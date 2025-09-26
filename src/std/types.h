//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

#if defined(__x86_64__) || defined(_M_X64)
typedef unsigned long long uintptr_t;
typedef unsigned long long size_t;
#else
typedef unsigned int uintptr_t;
typedef unsigned int size_t;
#endif

typedef char bool;
#define false 0
#define true 1

#define nullptr (void*)0
