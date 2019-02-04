//
//  alpha.cpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#include "../../old/devsys-old.h"
#include "emu/core.h"
#include "emu/debug.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Driver axp_sysDriver {
	"AXP",
	"AlphaServer",
	__FILE__,
	nullptr,

	// Configurations,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// Function calls
	nullptr
};
