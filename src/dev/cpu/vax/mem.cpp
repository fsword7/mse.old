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

// Aligned read access with physical address
uint32_t vax_cpuDevice::readpa(uint32_t pAddr, int size)
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
uint32_t vax_cpuDevice::readpl(uint32_t pAddr)
{
	if (pAddr < memSize)
		return MEML(pAddr >> 2);
	return 0;
}

// Aligned write access with physical address
void vax_cpuDevice::writepa(uint32_t pAddr, uint32_t data, int size)
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

// Console read/write access routines

#define cibLeft  cibData[0]
#define cibRight cibData[1]

uint32_t align[] = { 0xFFFFFFFF, 0x00FFFFFF, 0x0000FFFF, 0x000000FF };

// Flush instruction buffer
void vax_cpuDevice::flushci()
{
	cvAddr   = -1;
    cibAddr  = -1;
	cibCount = 0;
	cibLeft  = 0;
	cibRight = 0;
}

int vax_cpuDevice::readci(uint32_t vAddr, uint32_t *data, int size)
{
	int boff = vAddr & 03;
	int sc   = boff << 3;

	// Flush instruction buffer
	if (vAddr != cvAddr) {
		cvAddr   = vAddr;
		cibAddr  = -1;
		cibCount = 0;
	}

	if ((boff + size) > cibCount) {
		if (cibAddr == -1) {
			cibAddr  = (vAddr + cibCount) & ~03;
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
