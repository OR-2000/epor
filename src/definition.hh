#pragma once

#define CLOCKS_PER_US (2100)
#define CLOCKS_PER_MS (CLOCKS_PER_US * 1000)
#define CLOCKS_PER_S (CLOCKS_PER_US * 1000 * 1000)

#define KILO (1000)
#define MEGA (KILO * KILO)
#define GIGA (MEGA * KILO)

#define CACHE_LINE_SIZE (64)
#define CACHE_ALIGNED alignas(64)  // 64バイト境界にアライメントする

#define cpu_relax() asm volatile("rep; nop" ::: "memory");

#define IS_LONG(id) ((id) % 2 == 0)
// 0-111: Long(0-111)
// 112-223: Long(112-223), Short(224-335)