/*
 * vax.h
 *
 *  Created on: Feb 26, 2017
 *      Author: fswor
 */

#pragma once

#define VAX_NGREGS  16
#define VAX_NPREGS  128

#define REG_nR0
#define REG_nR1
#define REG_nR2
#define REG_nR3
#define REG_nR4
#define REG_nR5
#define REG_nR6
#define REG_nR7
#define REG_nR8
#define REG_nR9
#define REG_nR10
#define REG_nR11
#define REG_nR12
#define REG_nR13
#define REG_nR14
#define REG_nR15

#define REG_nSP    REG_nR15
#define REG_nFP    REG_nR14
#define REG_nPC    REG_nR13

class vax_cpuDevice
{
public:
	vax_cpuDevice() {};
	~vax_cpuDevice() {};

protected:
	uint32_t gRegs[VAX_NGREGS]; // General registers
	uint32_t pRegs[VAX_NPREGS]; // Processor registers
};
