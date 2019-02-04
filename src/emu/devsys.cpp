/*
 * system.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"
#include "emu/devcore.h"
#include "emu/console.h"

Device::Device(int _clsType)
: clsType(_clsType), driver(nullptr), parent(nullptr)
{
}

Device::~Device()
{
}

void Device::setSystemDevice(sysDevice *sdev)
{
	this->sdev = sdev;
#ifdef ENABLE_DEBUG
	dbg.setDevice(sdev);
#endif /* ENABLE_DEBUG */
}

Device *Device::findDevice(std::string devName)
{
	for (auto &&dev : devices) {
		if (devName == dev->getName())
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
		while ((++model)->name != nullptr) {
			if (model->name == sysName)
				return model;
		}
	}
	return nullptr;
}

cfgMemory *Device::getMemoryConfig() const
{
	if (driver == nullptr || driver->cfgMemory == nullptr)
		return nullptr;
	return driver->cfgMemory;
}

const Command *Device::getCommandTable() const
{
	if (driver == nullptr || driver->Commands == nullptr)
		return nullptr;
	return driver->Commands;
}

const Command *Device::getSetCommandTable() const
{
	if (driver == nullptr || driver->setCommands == nullptr)
		return nullptr;
	return driver->setCommands;
}

const Command *Device::getShowCommandTable() const
{
	if (driver == nullptr || driver->showCommands == nullptr)
		return nullptr;
	return driver->showCommands;
}

const Command *Device::getListCommandTable() const
{
	if (driver == nullptr || driver->listCommands == nullptr)
		return nullptr;
	return driver->listCommands;
}

int Device::setMemory(uint32_t)
{
	std::cerr << devName << ": Memory configuration not supported" << std::endl;
	return CMD_ERROR;
}

int Device::boot()
{
	std::cerr << devName << ": Command not supported" << std::endl;
	return CMD_ERROR;
}

int Device::stop()
{
	std::cerr << devName << ": Command not supported" << std::endl;
	return CMD_ERROR;
}

int Device::load(std::string, uint32_t)
{
	std::cerr << devName << ": Command not supported" << std::endl;
	return CMD_ERROR;
}

int Device::dump(uint32_t *, uint32_t, uint32_t)
{
	std::cerr << devName << ": Command not supported" << std::endl;
	return CMD_ERROR;
}

// ******************************************************************************

sysDevice::sysDevice()
: Device(CLS_SYSTEM)
{
	cpu.clear();
}

sysDevice::~sysDevice()
{
	cpu.clear();

	// Close all log files
	log.close(-1);
}

void sysDevice::addCPUDevice(cpuDevice *cpu)
{
	this->cpu.push_back(cpu);
	addDevice(cpu);
}

void sysDevice::addConsoleDevice(ttyDevice *cty)
{
	this->cty.push_back(cty);
	addDevice(cty);
}

cpuDevice *sysDevice::getCPUDevice(int idx)
{
	if (idx < 0 || idx >= cpu.size())
		return nullptr;
	return cpu[idx];
}

uint32_t sysDevice::readio(cpuDevice *, uint32_t, uint32_t)
{
	return 0;
}

void sysDevice::writeio(cpuDevice *, uint32_t, uint32_t, uint32_t)
{
}

// ******************************************************************************

ioDevice::ioDevice(int clsType)
: Device(clsType)
{
}

ioDevice::~ioDevice()
{
	// Close all log files
//	log.close(-1);
}

uint8_t ioDevice::read8(uint32_t)
{
	return 0;
}

uint16_t ioDevice::read16(uint32_t)
{
	return 0;
}

uint32_t ioDevice::read32(uint32_t)
{
	return 0;
}

uint64_t ioDevice::read64(uint32_t)
{
	return 0;
}

void ioDevice::write8(uint32_t, uint8_t)
{
}

void ioDevice::write16(uint32_t, uint16_t)
{
}

void ioDevice::write32(uint32_t, uint32_t)
{
}

void ioDevice::write64(uint32_t, uint64_t)
{
}

// ******************************************************************************

//extern Driver axp_sysDriver;
//extern Driver hsc_sysDriver;
//extern Driver ibm3x0_sysDriver;
//extern Driver ibm700_sysDriver;
//extern Driver pdp10_sysDriver;
//extern Driver pdp11_sysDriver;
//extern Driver vax_sysDriver;
//
//Driver *sysDrivers[] = {
//	&axp_sysDriver,
//	&hsc_sysDriver,
//	&ibm3x0_sysDriver,
//	&ibm700_sysDriver,
//	&pdp10_sysDriver,
//	&pdp11_sysDriver,
//	&vax_sysDriver,
//	// end of system driver table
//	nullptr
//};
//
//extern sysModel vax_sysModels[];
//
//sysModel *sysModels[] = {
//	vax_sysModels,
//
//	// null terminator
//	nullptr
//};
//
//extern Command mseCommands[];
//extern Command mseSetCommands[];
//extern Command mseShowCommands[];
//extern Command mseListCommands[];
//
//Driver mseDriver = {
//		"mse",
//		"Multi-System Emulator",
//		__FILE__,
//		nullptr,
//
//		// Configurations
//		nullptr,
//
//		// Command handlers
//		mseCommands,
//		mseSetCommands,
//		mseShowCommands,
//		mseListCommands,
//
//		// Function Calls
//		nullptr
//};
//
//void setSystemDrivers(Device *dev)
//{
//	dev->setDrivers(sysDrivers);
//	dev->setModels(sysModels);
//}
