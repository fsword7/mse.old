/*
 * vax.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/sysdrv.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Device vax_sysDevice {
	"VAX",
	"DEC VAX-11 Series"
	__FILE__,
};
