#pragma once

#define oxy_noret __attribute__((noreturn)) void
#define oxy_nobss __attribute__((section(".data")))
#define oxy_nosse __attribute__((optimize("no-tree-vectorize")))
