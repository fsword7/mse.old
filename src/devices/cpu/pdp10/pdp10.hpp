/*
 * pdp10.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 */

#once

#include "emu/core.hpp"
#include "emu/cpu/pdp10/word10.hpp"

#define AC_NBLKS      8
#define AC_NREGS    017

class Processor_PDP10
{
public:
	// Accumulators
	w10_t  acRegs[AC_NBLKS][AC_NREGS];
	// Current/Previous Accumulators
	w10_t *curReg; // Current AC block
	w10_t *prvReg; // Previous AC block

	Processor_PDP10()  {}
	~Processor_PDP10() {}

	virtual void execute();
};
