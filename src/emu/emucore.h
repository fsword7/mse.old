/*
 * emucore.h - main header
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <utility>
#include <exception>
#include <map>

#include <cstdio>
#include <cstddef>
#include <cstdarg>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <boost/algorithm/string.hpp>

// LSB_FIRST endianness (1 = little endianess, 0 = big endianess)
#define LSB_FIRST 1

typedef const char tag_t;

enum endian_t {
	endianLittle,
	endianBig
};

#if LSB_FIRST
constexpr const endian_t endianNative = endianLittle;
#else
constexpr const endian_t endianNative = endianBig;
#endif

// named function call parameters
#define FUNC(func) func, #func

#include "lib/util/delegate.h"
#include "emu/devdelegate.h"
#include "emu/device.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"
#include "emu/emumap.h"

#include "emu/dimem.h"

#include "emu/templates.h"

#include "emu/badcast.h"

// Data length
#define LN_BYTE  1
#define LN_WORD  2
#define LN_LONG  4
#define LN_QUAD  8
#define LN_OCTA  16

// Data/address shift count
#define SC_BYTE  0
#define SC_WORD  1
#define SC_LONG  2
#define SC_QUAD  3
#define SC_OCTA  4

#define MSK_BYTE 0x000000FF
#define MSK_WORD 0x0000FFFF
#define MSK_LONG 0xFFFFFFFF

#define SGN_BYTE 0x00000080
#define SGN_WORD 0x00008000
#define SGN_LONG 0x80000000

#define SIGN_B   0x80
#define SIGN_W   0x8000
#define SIGN_L   0x80000000


#define ALIGN_BYTE (~0 << 0) // Aligned byte addressing mask
#define ALIGN_WORD (~0 << 1) // Aligned word addressing mask
#define ALIGN_LONG (~0 << 2) // Aligned longword addressing mask
#define ALIGN_QUAD (~0 << 3) // Aligned quadword addressing mask

// Sign extension
#define SXTB(d) int8_t(d)
#define SXTW(d) int16_t(d)
#define SXTL(d) int32_t(d)
#define SXTQ(d) int64_t(d)

// Zero extension
#define ZXTB(d) uint8_t(d)
#define ZXTW(d) uint16_t(d)
#define ZXTL(d) uint32_t(d)
#define ZXTQ(d) uint64_t(d)


// Varying pair/scale 16-bit integer
union pair16_t {
#if LSB_FIRST
	struct { int8_t l, h; }  sb;
	struct { uint8_t l, h; } ub;
#else
	struct { int8_t h, l; }  sb;
	struct { uint8_t h, l; } ub;
#endif
	int16_t  sw;
	uint16_t uw;
};

// Varying pair/scale 32-bit integer
union pair32_t {
#if LSB_FIRST
	struct { int8_t l, h, h2, h3; }  sb;
	struct { uint8_t l, h, h2, h3; } ub;
	struct { int16_t l, h; }  sw;
	struct { uint16_t l, h; } uw;
#else
	struct { int8_t h3, h2, h, l; }  sb;
	struct { uint8_t h3, h2, h, l; } ub;
	struct { int16_t h, l; }  sw;
	struct { uint16_t h, l; } uw;
#endif
	int32_t  sd;
	uint32_t ud;
};

// Varying pair/scale 64-bit integer
union pair64_t {
#if LSB_FIRST
	struct { int8_t l, h, h2, h3, h4, h5, h6, h7; }  sb;
	struct { uint8_t l, h, h2, h3, h4, h5, h6, h7; } ub;
	struct { int16_t l, h, h2, h3; }  sw;
	struct { uint16_t l, h, h2, h3; } uw;
	struct { int32_t l, h; }  sd;
	struct { uint32_t l, h; } ud;
#else
	struct { int8_t h7, h6, h5. h4, h3, h2, h, l; }  sb;
	struct { uint8_t h7, h6. h5, h4, h3, h2, h, l; } ub;
	struct { int16_t h3, h2, h, l; }  sw;
	struct { uint16_t h3, h2, h, l; } uw;
	struct { int32_t h, l; }  sd;
	struct { uint32_t h, l; } ud;
#endif
	int64_t  sq;
	uint64_t uq;
};

// Varying scale unsigned integer
struct scale16_t {
	union {
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};

// Varying scale unsigned integer
struct scale32_t {
	union {
		uint32_t l; // Longword access
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};

// Varying scale unsigned integer
struct scale64_t {
	union {
		uint64_t q; // Quadword access
		uint32_t l; // Longword access
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};

void osdExit(std::string reason);

uint32_t getParity(uint32_t data, uint32_t odd);
void msePrintf(const char *format, ...);
