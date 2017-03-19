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
		if (drv->drvName == devName)
			return dev;
	}
	return nullptr;
}

Driver *Device::findDriver(std::string drvName)
{
	Driver *drv;

	for (int idx = 0; drivers[idx]; idx++) {
		drv = drivers[idx];
		if (drv->drvName == drvName)
			return drv;
	}
	return nullptr;
}

sysModel *Device::findModel(std::string sysName)
{
	sysModel *model;

	for (int idx1 = 0; sysModels[idx1]; idx1++) {
		model = sysModels[idx1];
		while (++model != nullptr) {
			if (model->name == sysName)
				return model;
		}
	}
	return nullptr;
}

int Device::load(std::string)
{
	std::cout << driver->drvName << ": Command not supported" << std::endl;
	return CMD_ERROR;
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

extern sysModel vax_sysModels[];

sysModel *sysModels[] = {
	vax_sysModels,

	// null terminator
	nullptr
};

extern Command mseCommands;
//extern Command mseSetCommands;
//extern Command mseShowCommands;

Driver mseDriver = {
		"mse",
		"Multi-System Emulator",
		__FILE__,
		nullptr,

		// Command handlers
		&mseCommands,
		nullptr,
		nullptr,

};

void setSystemDrivers(Device *dev)
{
	dev->setDrivers(sysDrivers);
	dev->setModels(sysModels);
}
