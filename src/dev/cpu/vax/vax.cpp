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
#include "dev/cpu/vax/opcodes.h"

vax_cpuDevice::vax_cpuDevice()
{
	buildOpcodes();
//	reset();
}

vax_cpuDevice::~vax_cpuDevice()
{
}

//void vax_cpuDevice::reset()
//{
//	// Initialize all working registers
//	for (int idx = 0; idx < VAX_nGREGS; idx++)
//		gRegs[idx].l = 0;
//}

int  vax_cpuDevice::boot()
{
	return 0;
}

void vax_cpuDevice::init()
{
}

void vax_cpuDevice::assignMemory(uint8_t *mem, uint32_t memSize)
{
	this->mem     = mem;
	this->memSize = memSize;
}

void vax_cpuDevice::setPCAddr(uint32_t pcAddr)
{
	REG_PC = pcAddr;
}

uint32_t vax_cpuDevice::readpr(uint32_t)
{
	return 0;
}

void vax_cpuDevice::writepr(uint32_t, uint32_t)
{
}

//#define CPU_CLASS vax_cpuDevice
//#include "dev/cpu/vax/executes.h"
