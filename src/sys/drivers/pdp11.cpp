/*
 * pdp11.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: fswor
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys-old.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Driver pdp11_sysDriver {
	"PDP11",
	"DEC PDP-11 Series",
	__FILE__,
	nullptr,

	// Configurations
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	nullptr
};
