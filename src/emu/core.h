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
