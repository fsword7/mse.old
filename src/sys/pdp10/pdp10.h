/*
 * pdp10.h
 *
 *  Created on: Mar 6, 2017
 *      Author: fswor
 */

#pragma once

#include "formats/dec/word10.h"

// EXE/SAV Format Specifications

// EXE header blocks
#define SAV_ID_ENTRY_VECTOR 01775
#define SAV_ID_DIRECTORY    01776
#define SAV_ID_END_BLOCK    01777

// Page flags
#define SAV_M_HIGH_SEG  0400000000000LL // Page is part of high segement
#define SAV_M_SHARABLE  0200000000000LL // Page is sharable
#define SAV_M_WRITABLE  0100000000000LL // Page is writable
#define SAV_M_CONCEALED 0040000000000LL // Page is concealed
#define SAV_M_SYM_TABLE 0020000000000LL // Page is part of symbol table
#define SAV_M_ALLOCATED 0010000000000LL // Page is allocated but zero

#define SAV_BLK_SIZE 512 // 512 words per block (777 words in octal)


class pdp10_sysDevice {
public:
	pdp10_sysDevice();
	~pdp10_sysDevice();

	inline uint36_t *getMemory() const { return mem; }

	int load(std::string fname);

	// memory access function calls
	uint36_t readp(uint32_t paddr);
	void writep(uint32_t paddr, uint36_t data);

private:
	uint32_t  memSize;
	uint36_t *mem;
};
