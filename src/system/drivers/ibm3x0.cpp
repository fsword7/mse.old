/*
 * ibm3x0.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

Device ibm3x0_sysDriver {
	"IBM3x0",
	"IBM 360/370/390 Series"
	__FILE__,
};
