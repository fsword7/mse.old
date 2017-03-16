/*
 * v11.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  V-11 (Scorpio) processor emulation
 */


#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/v11.h"


v11_cpuDevice::v11_cpuDevice()
{
}

v11_cpuDevice::~v11_cpuDevice()
{
}

void v11_cpuDevice::reset()
{
	// Initialize SID register
	pRegs[PR_SID] = (SID_ID|SID_CPUREV|SID_PATREV|SID_MBO|SID_UCODE);
}

#define CPU_V11
#define CPU_CLASS v11_cpuDevice
#include "dev/cpu/vax/executes.h"
