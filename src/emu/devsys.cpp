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
: driver(nullptr), parent(nullptr)
{
}

Device::~Device()
{
}

Device *Device::findDevice(std::string devName)
{
	for (auto &&dev : devices) {
		Driver *drv = dev->getDriver();
		if (drv->devName == devName)
			return dev;
	}
	return nullptr;
}

Driver *Device::findDriver(std::string drvName)
{
	Driver *drv;

	for (int idx = 0; drivers[idx]; idx++) {
		drv = drivers[idx];
		if (drv->devName == drvName)
			return drv;
	}
	return nullptr;
}

// ******************************************************************************

sysDevice::sysDevice()
{
}

sysDevice::~sysDevice()
{
}

// ******************************************************************************

extern Driver axp_sysDriver;
extern Driver hsc_sysDriver;
extern Driver ibm3x0_sysDriver;
extern Driver ibm700_sysDriver;
extern Driver pdp10_sysDriver;
extern Driver pdp11_sysDriver;
extern Driver vax_sysDriver;

Driver *sysDrivers[] = {
	&axp_sysDriver,
	&hsc_sysDriver,
	&ibm3x0_sysDriver,
	&ibm700_sysDriver,
	&pdp10_sysDriver,
	&pdp11_sysDriver,
	&vax_sysDriver,
	// end of system driver table
	nullptr
};

void setSystemDrivers(Device *dev)
{
	dev->setDrivers(sysDrivers);
}

// Usage: create [device] <options...>
int cmdCreate(Console *con, Device *dev, args_t &args)
{
	Driver *drv;

	// Check number of arguments
	if (args.size() < 3) {
		std::cout << "Usage: " << args[0] << " [device] [driver]" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	if (dev->findDevice(args[1]) != nullptr) {
		std::cout << args[1] << ": name already taken or device not found." << std::endl;
		return CMD_OK;
	}

	// find available driver by using name
	drv = dev->findDriver(args[2]);
	if (drv == nullptr) {
		std::cout << args[2] << ": driver not found." << std::endl;
		return CMD_OK;
	}

	return CMD_OK;
}
