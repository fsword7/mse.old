/*
 * machine.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */


#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/emumap.h"
#include "emu/romloader.h"
#include "emu/machine.h"

machine::machine(const system_config *_config)
: config(*_config),
  sysDevice(config.systemDevice()),
  romLoad(nullptr),
  memory(this)
{
}

machine::~machine()
{
}

void machine::setDeviceName(std::string name)
{
	config.systemDevice()->setDeviceName(name);
}

std::string machine::getDeviceName()
{
	return config.systemDevice()->deviceName();
}

// Running machine initialization
void machine::init()
{
	// Resolve objects that can be used for memory maps.
//	for (device_t &device : device_iterator(sysDevice))
//		device.resolve_pre_map();

	// Loading ROM images
	romLoad = new rom_load_manager(this);

	// Initialize memory on all devices.
	memory.init();

	// Resolve objects that are created for memory maps.
//	for (device_t &device : device_iterator(sysDevice))
//		device.resolve_post_map();

}

void machine::execute()
{
}
