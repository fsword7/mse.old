/*
 * vax.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/devsys.h"
#include "sys/vax/vax.h"

//GROUP(alpha)
//SYSTEM(as400, alpha,   "AlphaServer 400")

vax_sysDevice::vax_sysDevice()
: memSize(0), mem(nullptr)
{
}

vax_sysDevice::~vax_sysDevice()
{
	if (mem != nullptr)
		delete mem;
}

int vax_sysDevice::load(std::string fname)
{
	return CMD_OK;
}

Driver vax_sysDriver {
	"VAX",
	"DEC VAX-11 Series"
	__FILE__,
	nullptr,

	// Command handlers
	nullptr,
	nullptr,
	nullptr,
};
