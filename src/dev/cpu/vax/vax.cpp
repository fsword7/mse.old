/*
 * vax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"

vax_cpuDevice::vax_cpuDevice()
{
}

vax_cpuDevice::~vax_cpuDevice()
{
}

void vax_cpuDevice::reset()
{
	// Initialize all working registers
	for (int idx = 0; idx < VAX_nGREGS; idx++)
		gRegs[idx].l = 0;
}

#define CPU_CLASS vax_cpuDevice
#include "dev/cpu/vax/executes.h"
