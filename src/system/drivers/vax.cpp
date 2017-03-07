/*
 * vax.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

sysDriver vax_sysDriver {
	"VAX",
	"DEC VAX-11 Series"
	__FILE__,
	nullptr,

};
