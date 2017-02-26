/*
 * ibm300.h
 *
 *  Created on: Feb 26, 2017
 *      Author: Timothy Stark
 *
 *  Emulates System/360, System/370, System/380, ESA/390 and zArch processor
 *
 */

#pragma once

#define  I3x0_NGREGS  16 // General registers
#define  I3x0_NFREGS  16 // Floating registers
#define  I3x0_NCREGS  16 // Control registers

class i300_cpuDevice
{
public:
	i300_cpuDevice() {};
	~i300_cpuDevice() {};

private:
	// Processor registers
	uint64_t gRegs[I3x0_NGREGS];
	uint64_t fRegs[I3x0_NFREGS];
	uint64_t cRegs[I3x0_NCREGS];
};
