/*
 * engine.cpp
 *
 *  Created on: Jan 13, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/device.h"
#include "emu/devsys2.h"
#include "emu/driver.h"
#include "emu/syslist.h"
#include "emu/engine.h"

using namespace std;

vector<device_t> system_engine::machine;

device_t *system_engine::find(string drvName)
{
	if (machine.empty())
		return nullptr;
	for (auto &sys : machine) {
		if (sys.name() == drvName)
			return &sys;
	}
	return nullptr;
}

device_t *system_engine::create(string devName, string sysName)
{
	system_list    syslist;
	const system_driver *driver;
	device_t      *sys = nullptr;

	// Named system must be non-existent first
	if (find(devName) != nullptr)
		return nullptr;

	// Find system from system list database
	driver = syslist.find(sysName);
	if (driver != nullptr)
		return nullptr;

	// Initialize system device
	driver->create(*sys);
	driver->init(*sys);

	if (sys != nullptr)
		machine.push_back(*sys);
	return sys;
}
