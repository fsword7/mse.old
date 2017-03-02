//
//  pdp10.cpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#include "emu/core.h"
#include "emu/sysdrv.h"

GROUP(pdp10)
SYSTEM(dec2020, pdp10,   "DECsystem2020")

Device pdp10_sysDevice {
	"PDP10",
	"DECsystem-10",
	__FILE__,
};
