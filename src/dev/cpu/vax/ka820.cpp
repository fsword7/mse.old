/*
 * ka820.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/ka820.h"



ka820_cpuDevice::ka820_cpuDevice()
{
}

ka820_cpuDevice::~ka820_cpuDevice()
{
}

void ka820_cpuDevice::reset()
{
	// Initialize SID register
	pRegs[PR_SID] = (SID_ID|SID_CPUREV|SID_PATREV|SID_MBO|SID_UCODE);
}



