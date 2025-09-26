//
// Created by felix on 9/26/2025.
//

// COPYRIGHT Fentanyl LLC 2025
// v0.0.1-beta

// MIT LICENSE

#pragma once

#define oxy_noret __attribute__((noreturn)) void
#define oxy_nobss __attribute__((section(".data")))
#define oxy_nosse __attribute__((optimize("no-tree-vectorize")))
