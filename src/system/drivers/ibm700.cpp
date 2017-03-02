/*
 * ibm700.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/sysdrv.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Device ibm700_sysDevice {
	"IBM700",
	"IBM 700 Series"
	__FILE__,
};
