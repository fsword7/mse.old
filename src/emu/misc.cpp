/*
 * misc.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: fswor
 */

#include "emu/emucore.h"

// Generate parity bit
uint32_t getParity(uint32_t data, uint32_t odd)
{
	for (; data != 0; data >>= 1)
		if (data & 1)
			odd ^= 1;
	return odd;
}
