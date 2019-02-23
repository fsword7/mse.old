/*
 * devsys2.cpp - system driver device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys.h"

system_device::system_device(const system_config &config, const device_type_base &type,
		tag_t *tag, uint64_t clock)
: device_t(config, type, tag, nullptr, clock),
  sysDriver(nullptr)
{

}

system_device::~system_device()
{
}

void system_device::setSystemDriver(const system_driver *driver)
{
	assert(sysDriver == nullptr);

	sysDriver = driver;

}

void system_device::devConfigure(system_config &config)
{
	assert(sysDriver != nullptr);
	sysDriver->configure(config, *this);
}

// init - default implementation does nothing
void system_device::init()
{
}

