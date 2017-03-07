/*
 * pdp11.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devsys.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Device pdp11_sysDriver {
	"PDP11",
	"DEC PDP-11 Series"
	__FILE__,
};
