/*
 * engine.cpp
 *
 *  Created on: Jan 13, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/device.h"
#include "emu/driver.h"
#include "emu/syslist.h"
#include "emu/engine.h"
#include "emu/devsys.h"
#include "emu/machine.h"

using namespace std;

vector<machine> system_engine::machines;

//device_t *system_engine::find(string drvName)
//{
//	if (machine.empty())
//		return nullptr;
//	for (auto &sys : machine) {
//		if (sys.name() == drvName)
//			return &sys;
//	}
//	return nullptr;
//}

void system_engine::create(string devName, string sysName)
{
	system_list    syslist;
	const system_driver *driver;
	const system_config *config;
	machine *sys = nullptr;

	// Named system must be non-existent first
//	if (find(devName) != nullptr)
//		return nullptr;

	// Find system from system list database
	driver = syslist.find(sysName);
	if (driver == nullptr) {
		std::cout << devName << ": System " << sysName << " not found" << std::endl;
		return;
	}

	config = new system_config(*driver);
	sys = new machine(config);

	if (sys != nullptr)
		machines.push_back(*sys);
}
