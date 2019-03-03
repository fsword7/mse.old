/*
 * machine.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */


#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/romloader.h"
#include "emu/machine.h"

machine::machine(const system_config *_config)
: sysConfig(*_config),
  sysDriver(sysConfig.driver()),
  sysDevice(sysConfig.systemDevice()),
  romLoader(nullptr),
  sysMemory(this)
{
	// Assign this to all devices
	device_iterator iter(*sysDevice);
	for (device_t &dev : iter)
		dev.setMachine(this);
}

machine::~machine()
{
}

void machine::setDeviceName(tag_t *name)
{
	sysConfig.systemDevice()->setDeviceName(name);
}

tag_t *machine::getDeviceName()
{
	return sysConfig.systemDevice()->deviceName();
}

// Running machine initialization
void machine::init(const cty_t &cty)
{
	// Resolve objects that can be used for memory maps.
//	for (device_t &device : device_iterator(sysDevice))
//		device.resolve_pre_map();

	// Loading ROM images
	romLoader = new rom_loader(this, cty);

	// Initialize memory on all devices.
	sysMemory.init();

	// Resolve objects that are created for memory maps.
//	for (device_t &device : device_iterator(sysDevice))
//		device.resolve_post_map();

}

void machine::execute()
{
}
