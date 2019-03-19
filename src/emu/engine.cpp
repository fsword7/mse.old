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
#include "emu/devcpu.h"
#include "emu/devsys.h"
#include "emu/machine.h"

using namespace std;

vector<machine> system_engine::machines;

machine *system_engine::find(string drvName)
{
	if (machines.empty())
		return nullptr;
	for (auto &sys : machines) {
		if (sys.getDeviceName() == drvName)
			return &sys;
	}
	return nullptr;
}

device_t *system_engine::findDevice(device_t &sys, string devName) const
{
	for (device_t &dev : device_iterator(sys)) {
		if (string(dev.deviceName()) == devName)
			return &dev;
	}
	return nullptr;
}

void system_engine::create(string devName, string sysName)
{
	system_list    syslist;
	const system_driver *driver;
	const system_config *config;
	machine *sys = nullptr;

	// Named system must be non-existent first
	if (find(devName) != nullptr) {
		cty.printf("%s: system already created\n", devName.c_str());
		return;
	}

	// Find system from system list database
	driver = syslist.find(sysName);
	if (driver == nullptr) {
		cty.printf("%s: System '%s' not found\n", devName.c_str(), sysName.c_str());
		return;
	}

	config = new system_config(*driver);
	sys = new machine(config);
	char *name = strdup(devName.c_str());
	sys->setDeviceName(name);

	sys->start(cty);

	if (sys != nullptr)
		machines.push_back(*sys);
}

void system_engine::list()
{
	for (auto &machine : machines) {
		device_t *sys = machine.getSystemDevice();

//		std::cout << sys->deviceName() << "  " << sys->tagName() << "  "
//				  << sys->shortName() << " (" << sys->fullName() << ")"
//				  << std::endl;

		for (device_t &dev : device_iterator(*sys)) {
			cty.printf("%-10s %-10s %-10s (%s)\n", dev.deviceName(),
					dev.tagName(), dev.shortName(), dev.fullName());
		}
	}
}

void system_engine::start(int argc, args_t &args)
{
	machine			*machine;
	device_t		*sys, *dev;

	// find named machine device
	if ((machine = find(args[1])) == nullptr) {
		cty.printf("%s: System '%s' not found\n", args[0].c_str(), args[1].c_str());
		return;
	}

	// Start system device as initialization
	machine->start(cty);
}

void system_engine::disassemble(int argc, args_t &args)
{
	machine			*machine;
	device_t		*sys, *dev;

	// Check number of arguments
	if (args.size() < 4) {
		cty.printf("Usage: %s <system> <device> <start[-end]> [len]", args[0]);
		return;
	}

	// find named machine device
	if ((machine = find(args[1])) == nullptr) {
		cty.printf("%s: System '%s' not found\n", args[0].c_str(), args[1].c_str());
		return;
	}
	sys = machine->getSystemDevice();

	if ((dev = findDevice(*sys, args[2])) == nullptr) {
		cty.printf("%s: Device '%s' not found\n", args[0].c_str(), args[2].c_str());
		return;
	}

	di_debug *debug;
	if (!dev->hasInterface(debug)) {
		cty.printf("%s: Do not have debug interface\n", dev->deviceName());
		return;
	}

	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;

	sscanf(args[3].c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args[3].c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 4) {
			sscanf(args[4].c_str(), "%x", &eAddr);
			eAddr = sAddr + eAddr - 1;
		} else if (eAddr == -1)
			eAddr = sAddr + 0x140 - 1;
	}

	while (sAddr <= eAddr) {
		sAddr += debug->disassemble(&cty, sAddr);
	}
}

void system_engine::dump(int argc, args_t &args)
{
	machine			*machine;
	device_t		*sys, *dev;

	// Check number of arguments
	if (args.size() < 4) {
		cty.printf("Usage: %s <system> <device> <start[-end]> [len]", args[0]);
		return;
	}

	// find named machine device
	if ((machine = find(args[1])) == nullptr) {
		cty.printf("%s: System '%s' not found\n", args[0].c_str(), args[1].c_str());
		return;
	}
	sys = machine->getSystemDevice();

	if ((dev = findDevice(*sys, args[2])) == nullptr) {
		cty.printf("%s: Device '%s' not found\n", args[0].c_str(), args[2].c_str());
		return;
	}

	di_memory *memory;
	if (!dev->hasInterface(memory)) {
		cty.printf("%s: Do not have memory interface\n", dev->deviceName());
		return;
	}
	mapAddressSpace &space = memory->getAddressSpace(AS_PROGRAM);

	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;

	sscanf(args[3].c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args[3].c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 4) {
			sscanf(args[4].c_str(), "%x", &eAddr);
			eAddr = sAddr + eAddr - 1;
		} else if (eAddr == -1)
			eAddr = sAddr + 0x140 - 1;
	}

	int       idx;
	char      line[256], lasc[32];
	char      *lptr, *pasc;
	uint32_t  data;
	uint32_t  sts;

	while (sAddr <= eAddr) {
		lptr = line;
		pasc = lasc;
		lptr += sprintf(lptr, "%08X: ", sAddr);
		for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++) {
			data = space.read8(sAddr++);
			lptr += sprintf(lptr, "%02X%c", data, (idx == 7) ? '-' : ' ');
			*pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
		}
		*pasc = '\0';
		*lptr = '\0';

		cty.printf("%s |%-16s|\n", line, lasc);
	}

}

void system_engine::dumpr(int argc, args_t &args)
{
	machine			*machine;
	device_t		*dev;
	mapMemoryRegion	*region;

	// Check number of arguments
	if (args.size() < 4) {
		cty.printf("Usage: %s <system> <region> <start[-end]> [len]", args[0]);
		return;
	}

	// find named machine device
	if ((machine = find(args[1])) == nullptr) {
		cty.printf("%s: System '%s' not found\n", args[0].c_str(), args[1].c_str());
		return;
	}
	dev = machine->getSystemDevice();

	// find named region area
	if ((region = dev->mapGetMemoryRegion(args[2])) == nullptr) {
		cty.printf("%s: Region '%s' not found\n", args[0].c_str(), args[2].c_str());
		return;
	}

	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;

	sscanf(args[3].c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args[3].c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 4) {
			sscanf(args[4].c_str(), "%x", &eAddr);
			eAddr = sAddr + eAddr - 1;
		} else if (eAddr == -1)
			eAddr = sAddr + 0x140 - 1;
	}

	int       idx;
	char      line[256], lasc[32];
	char      *lptr, *pasc;
	uint32_t  data;
	uint32_t  sts;
	uint8_t	  *base = region->base();

	while (sAddr <= eAddr) {
		lptr = line;
		pasc = lasc;
		lptr += sprintf(lptr, "%08X: ", sAddr);
		for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++) {
			data = base[sAddr++];
			lptr += sprintf(lptr, "%02X%c", data, (idx == 7) ? '-' : ' ');
			*pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
		}
		*pasc = '\0';
		*lptr = '\0';

		cty.printf("%s |%-16s|\n", line, lasc);
	}

}
