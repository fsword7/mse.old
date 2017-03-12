/*
 * hsc.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/devsys.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Driver hsc_sysDriver {
	"HSC",
	"DEC HSC Series"
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,
};


