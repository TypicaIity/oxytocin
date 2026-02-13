#pragma once

#include "int/isr.h"

void timer_init(uint32_t freq);

void timer_sleep(uint32_t ticks);

size_t timer_getUptimeMs();
