/*
 * machine.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */


#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/emumap.h"
#include "emu/machine.h"

machine::machine(const system_config *_config)
: config(*_config),
  sysDevice(config.systemDevice()),
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
	// Initialize memory on all devices.
	memory.init();
}

void machine::execute()
{
}
