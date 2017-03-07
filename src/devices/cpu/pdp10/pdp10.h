/*
 * pdp10.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 */

#once

#include <emu/core.h>
#include "formats/dec/word10.h"

#define AC_NBLKS      8
#define AC_NREGS    017

class pdp10_cpuDevice : public cpuDevice
{
public:
	// Accumulators
	w10_t  acRegs[AC_NBLKS][AC_NREGS];
	// Current/Previous Accumulators
	w10_t *curReg; // Current AC block
	w10_t *prvReg; // Previous AC block

	pdp10_cpuDevice()  {}
	virtual ~pdp10_cpuDevice() {}

	virtual void execute();
};
