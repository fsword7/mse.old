/*
 * pdp10.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 */

#once

#include "emu/devcpu.h"
#include "formats/dec/word10.h"

#define AC_NBLKS      8
#define AC_NREGS    017

class pdp10_cpu_base : public cpu_device
{
public:
	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	// Accumulators
	w10_t  acRegs[AC_NBLKS][AC_NREGS];
	// Current/Previous Accumulators
	w10_t *curReg; // Current AC block
	w10_t *prvReg; // Previous AC block

	pdp10_cpu_base()  {}
	virtual ~pdp10_cpu_base() {}

	virtual void execute();
};
