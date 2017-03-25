#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

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
