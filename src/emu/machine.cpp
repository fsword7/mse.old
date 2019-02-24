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
  romLoader(nullptr),
  memory(this)
{
}

machine::~machine()
{
}

void machine::setDeviceName(tag_t *name)
{
	config.systemDevice()->setDeviceName(name);
}

tag_t *machine::getDeviceName()
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
	romLoader = new rom_loader(this);

	// Initialize memory on all devices.
	memory.init();

	// Resolve objects that are created for memory maps.
//	for (device_t &device : device_iterator(sysDevice))
//		device.resolve_post_map();

}

void machine::execute()
{
}
