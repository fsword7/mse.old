/*
 * sysconfig.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"


system_config::system_config(const system_driver &driver)
: sysDriver(driver), sysDevice(nullptr)
{
//	addDevice("root", driver.type);
}

device_t *system_config::addDevice(const char *tag, device_type type)
{
	return nullptr;
}
