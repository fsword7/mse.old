/*
 * system.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/devcore.h"
#include "emu/console.h"

Device::Device()
{
}

Device::~Device()
{
}

sysDevice::sysDevice()
{
}

sysDevice::~sysDevice()
{
}

// ******************************************************************************

extern sysDriver axp_sysDriver;
extern sysDriver ibm3x0_sysDriver;
extern sysDriver ibm700_sysDriver;
extern sysDriver pdp10_sysDriver;
extern sysDriver pdp11_sysDriver;
extern sysDriver vax_sysDriver;

sysDriver *sysList[6] = {
	&axp_sysDriver,
	&ibm3x0_sysDriver,
	&ibm700_sysDriver,
	&pdp10_sysDriver,
	&pdp11_sysDriver,
	&vax_sysDriver
};

// Usage: create [device] <options...>
int cmdCreate(Console *con, args_t &args)
{
	appCore *app;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " [device] <options...>" << std::endl;
		return CMD_OK;
	}
	app = con->getSystem();


	return CMD_OK;
}
