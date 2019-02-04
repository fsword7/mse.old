/*
 * ibm700.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "../../old/devsys-old.h"
#include "emu/core.h"
#include "emu/debug.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Driver ibm700_sysDriver {
	"IBM700",
	"IBM 700 Series",
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
