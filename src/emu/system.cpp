/*
 * system.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/system.h"
#include "emu/sysdrv.h"
#include "emu/console.h"

System::System()
{
}

System::~System()
{
}

// ******************************************************************************

extern Device axp_sysDevice;
extern Device ibm3x0_sysDevice;
extern Device ibm700_sysDevice;
extern Device pdp10_sysDevice;
extern Device vax_sysDevice;

Device *sysList[5] = {
	&axp_sysDevice,
	&ibm3x0_sysDevice,
	&ibm700_sysDevice,
	&pdp10_sysDevice,
	&vax_sysDevice
};

// Usage: create [device] <options...>
int cmdCreate(Console *con, args_t &args)
{

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " [device] <options...>" << std::endl;
		return CMD_OK;
	}

	return CMD_OK;
}
