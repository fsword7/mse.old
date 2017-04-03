/*
 * mem.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: Timothy Stark
 *
 *  Memory access routines
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

#define MEMB(addr) ((uint8_t *)mem)[addr]
#define MEMW(addr) ((uint16_t *)mem)[addr]
#define MEML(addr) ((uint32_t *)mem)[addr]

static const uint32_t align[] =
	{ 0xFFFFFFFF, 0x00FFFFFF, 0x0000FFFF, 0x000000FF };
static const uint32_t insert[] =
	{ 0x00000000, 0x000000FF, 0x0000FFFF, 0x00FFFFFF };

// Aligned longword read access with physical address
uint32_t vax_cpuDevice::readpl(uint32_t pAddr)
{
	if (pAddr < memSize)
		return MEML(pAddr >> 2);
	return 0;
}

// Aligned read access with physical address
uint32_t vax_cpuDevice::readp(uint32_t pAddr, int size)
{
	if (pAddr < memSize) {
		if (size == OPR_LONG)
			return MEML(pAddr >> 2);
		if (size == OPR_WORD)
			return MEMW(pAddr >> 1);
		return MEMB(pAddr);
	}
	return 0;
}

// Aligned longword read access with physical address
void vax_cpuDevice::writepl(uint32_t pAddr, uint32_t data)
{
	if (pAddr < memSize)
		MEML(pAddr >> 2) = data;
}

// Aligned write access with physical address
void vax_cpuDevice::writep(uint32_t pAddr, uint32_t data, int size)
{
	if (pAddr < memSize) {
		if (size == OPR_LONG)
			MEML(pAddr >> 2) = data;
		if (size == OPR_WORD)
			MEMW(pAddr >> 1) = data;
		else
			MEMB(pAddr) = data;
	}
}


uint8_t vax_cpuDevice::readvb(uint32_t vAddr, uint32_t acc)
{
	return 0;
}

uint16_t vax_cpuDevice::readvw(uint32_t vAddr, uint32_t acc)
{
	return 0;
}

uint32_t vax_cpuDevice::readvl(uint32_t vAddr, uint32_t acc)
{
	return 0;
}

uint32_t vax_cpuDevice::readv(uint32_t vAddr, uint32_t size, uint32_t acc)
{
	uint32_t pAddr, pAddr1;
	uint32_t dl, dh;
	int      sc;

	pAddr = vAddr & paMask;
//	printf("mem: Read %08X (scale %d)\n", pAddr, size);

	// Check physical address is aligned
	if ((pAddr & (size - 1)) == 0) {
//		printf("mem: Aligned %08X => %08X\n", pAddr, readp(pAddr, size));
		return readp(pAddr, size);
	}

	pAddr1 = (pAddr + size) & paMask;

	// Read 16/32-bit data in unaligned address
	// for crossing a page, etc.
	if (size < LN_LONG) {
		if ((pAddr & 03) == 1)
			return uint16_t(readpl(pAddr) >> 8);
		else {
			dl = readpl(pAddr);
			dh = readpl(pAddr1);
			return uint16_t((dl >> 24) | (dh << 8));
		}
	} else {
		dl = readpl(pAddr);
		dh = readpl(pAddr1);
		sc = (pAddr & 03) << 3;
		return (dl >> sc) | (dh << (32 - sc));
	}

	return 0;
}


void vax_cpuDevice::writevb(uint32_t vAddr, uint8_t data, uint32_t acc)
{
}

void vax_cpuDevice::writevw(uint32_t vAddr, uint16_t data, uint32_t acc)
{
}

void vax_cpuDevice::writevl(uint32_t vAddr, uint32_t data, uint32_t acc)
{
}

void vax_cpuDevice::writev(uint32_t vAddr, uint32_t data, uint32_t size, uint32_t acc)
{
	uint32_t pAddr, pAddr1;
	uint32_t dl, dh;
	int      sc, boff;

	pAddr = vAddr & paMask;

	// Check physical address is aligned
	if ((pAddr & (size - 1)) == 0) {
		writep(pAddr, data, size);
		return;
	}

	pAddr1 = (pAddr + size) & paMask;

	// Write 16/32-bit data in unaligned address
	// for crossing a page, etc.
	boff = pAddr & 03;
	dl = readpl(pAddr);
	if (size < LN_LONG) {
		// Unaligned word (16-bit words)
		if (boff == 1) {
			dl = (dl & 0xFF0000FF) | (uint16_t(data) << 8);
			writepl(pAddr, dl);
		} else {
			dh = readpl(pAddr1);
			dl = (dl & 0x00FFFFFF) | (data << 24);
			dh = (dh & 0xFFFFFF00) | uint8_t(data >> 8);
			writepl(pAddr, dl);
			writepl(pAddr1, dh);
		}
	} else {
		// Unaligned longword (32-bit words)
		dh = readpl(pAddr1);
		sc = boff << 3;
		dl = (dl & insert[boff])  | (data << sc);
		dh = (dh & ~insert[boff]) | (data >> (32 - sc));
		writepl(pAddr, dl);
		writepl(pAddr1, dh);
	}
}

// Flush instruction buffer
void vax_cpuDevice::flushvi()
{
    ibpAddr   = ~0;
	ibCount   = 0;
	ibData[0] = 0;
	ibData[1] = 0;
}

// Instruction read access with current PC address (unaligned)
uint32_t vax_cpuDevice::readvi(int size)
{
	int      boff = REG_PC & 03;
	int      sc   = boff << 3;
	uint32_t pAddr, data;

	if ((boff + size) > ibCount) {
		if (ibpAddr == ~0) {
			pAddr    = REG_PC & paMask;
			ibpAddr  = (pAddr + ibCount) & ~03;
		}
		ibData[ibCount >> SC_LONG] = readpl(ibpAddr);
		ibpAddr += LN_LONG;
		ibCount += LN_LONG;
	}

	// Extract data from aligned longwords
	if (size == LN_BYTE)
		data = (ibData[0] >> sc) & MSK_BYTE;
	else if (size == LN_WORD) {
		data = (ibData[0] >> sc) & MSK_WORD;
		if (boff == 3)
			data |= (ibData[1] << (32 - sc)) & MSK_WORD;
	} else if (boff == 0)
		data = ibData[0];
	else
		data = (ibData[0] >> sc) | (ibData[1] << (32 - sc));

	// Ahead access to next data
	REG_PC += size;
	if ((boff + size) >= LN_LONG) {
		ibData[0] = ibData[1];
		ibCount  -= LN_LONG;
	}

	return data;
}

// Console read/write access routines

#define cibLeft  cibData[0]
#define cibRight cibData[1]


// Flush instruction buffer
void vax_cpuDevice::flushci()
{
	cvAddr   = ~0;
    cibAddr  = ~0;
	cibCount = 0;
	cibLeft  = 0;
	cibRight = 0;
}

int vax_cpuDevice::readci(uint32_t vAddr, uint32_t *data, int size)
{
	int      boff = vAddr & 03;
	int      sc   = boff << 3;
	uint32_t pAddr;

	// Flush instruction buffer
	if (vAddr != cvAddr) {
		cvAddr   = vAddr;
		cibAddr  = ~0;
		cibCount = 0;
	}

	if ((boff + size) > cibCount) {
		if (cibAddr == ~0) {
			pAddr    = vAddr & paMask;
			cibAddr  = (pAddr + cibCount) & ~03;
		}
		cibData[cibCount >> SC_LONG] = readpl(cibAddr);
		cibAddr  += LN_LONG;
		cibCount += LN_LONG;
	}

	// Extract data from aligned longwords
	if (size == LN_BYTE)
		*data = (cibLeft >> sc) & MSK_BYTE;
	else if (size == LN_WORD) {
		*data = (cibLeft >> sc) & MSK_WORD;
		if (boff == 3)
			*data |= (cibRight << (32 - sc)) & MSK_WORD;
	} else if (boff == 0)
		*data = cibLeft;
	else
		*data = (cibLeft >> sc) | (cibRight << (32 - sc));

	// Ahead access to next data
	cvAddr += size;
	if ((boff + size) >= LN_LONG) {
		cibLeft = cibRight;
		cibCount -= LN_LONG;
	}

	return 0;
}

int vax_cpuDevice::readc(uint32_t vAddr, uint32_t *data, int size)
{
	return 0;
}
