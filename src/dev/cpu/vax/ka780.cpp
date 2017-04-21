/*
 * ka780.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  KA780/KA785 CPU series
 *
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/ka780.h"
#include "dev/cpu/vax/opcodes.h"



ka780_cpuDevice::ka780_cpuDevice()
{
}

ka780_cpuDevice::~ka780_cpuDevice()
{
}

void ka780_cpuDevice::reset()
{
	// Initialize SID register
	ipReg[IPR_nSID] = (SID_ID|SID_ECO|SID_PLANT);
	// Set specific serial number
	ipReg[IPR_nSID] |= SID_SN;
}

void ka780_cpuDevice::mfpr()
{
}

void ka780_cpuDevice::mtpr()
{
}

#define CPU_KA780
#define CPU_CLASS ka780_cpuDevice
#include "dev/cpu/vax/executes.h"
