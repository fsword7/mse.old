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

#include <cstdio>
#include <cstddef>
#include <cstdarg>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <boost/algorithm/string.hpp>

// named function call parameters
#define FUNC(func) func, #func

#include "lib/util/delegate.h"
#include "emu/devdelegate.h"
#include "emu/device.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"
#include "emu/emumap.h"


typedef const char tag_t;

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
