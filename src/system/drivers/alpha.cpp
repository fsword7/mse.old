//
//  alpha.cpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#include "emu/core.h"
#include "emu/sysdrv.h"

GROUP(alpha)
SYSTEM(as400, alpha,   "AlphaServer 400")

Device axp_sysDevice {
	"AXP",
	"AlphaServer"
	__FILE__,
};
