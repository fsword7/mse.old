/*
 * vax.cpp
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys-old.h"
#include "emu/devcpu.h"
#include "emu/console.h"
#include "dev/cpu/vax/vax.h"
#include "dev/comm/dec/cty.h"
#include "sys/vax/vax.h"

vax_sysDevice::vax_sysDevice()
: memSize(0), mem(nullptr), cty(nullptr)
{
}

vax_sysDevice::~vax_sysDevice()
{
	if (mem != nullptr)
		delete mem;
}

int vax_sysDevice::setMemory(uint32_t size)
{
	if (mem != nullptr)
		delete mem;
	if (size > 0) {
		memSize = size;
		mem = new uint8_t[size];
	} else {
		memSize = 0;
		mem = nullptr;
	}

	for (auto &&cdev : cpu)
		((vax_cpuDevice *)cdev)->assignMemory(mem, memSize);

	return CMD_OK;
}

int vax_sysDevice::load(std::string fname, uint32_t sAddr)
{
	uint32_t addr, eAddr;

	std::ifstream in(fname, std::ios::binary);

	addr = sAddr;
	while (in.read((char *)&mem[addr], 512)) {
		addr += 512;
	}
	eAddr = addr;

	in.close();

	printf("%s: Loaded %08X to %08X (%08X bytes)\n",
			fname.c_str(), sAddr, eAddr, eAddr - sAddr);
	return CMD_OK;
}

// *****************************************************************************************

// Usage: boot <device> [options...]
static int cmdBoot(Console *cty, Device *sdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <device> [options...]" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = sdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

	dev->boot();

	return CMD_OK;
}

// Usage: start <device> [options...]
static int cmdStart(Console *cty, Device *sdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <device> [options...]" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = sdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

//	dev->start();

	return CMD_OK;
}

// Usage: stop <device>
static int cmdStop(Console *cty, Device *sdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <device>" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = sdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

	dev->stop();

	return CMD_OK;
}

// Usage: disasm <start> [end]
static int cmdDisasm(Console *cty, Device *sdev, args_t &args)
{
	uint32_t  sAddr, eAddr = -1;
	int       count = -1;
	char     *strAddr;
	vax_cpuDevice *cpu;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <start[-end]> [len]" << std::endl;
		return CMD_OK;
	}


	sscanf(args[1].c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args[1].c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 2)
			sscanf(args[2].c_str(), "%d", &count);
		else
			count = 20;
	}

	cpu = (vax_cpuDevice *)((sysDevice *)sdev)->getCPUDevice(0);

	// Display disassembly listing.
	if (count > 0) {
		while (count--)
			sAddr += cpu->disasm(cty, sAddr);
	} else {
		while (sAddr < eAddr)
			sAddr += cpu->disasm(cty, sAddr);
	}

	return CMD_OK;
}

// Usage: dump <start> [end]
static int cmdDump(Console *cty, Device *sdev, args_t &args)
{
	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;
	vax_cpuDevice *cpu;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <start[-end]> [len]" << std::endl;
		return CMD_OK;
	}

	sscanf(args[1].c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args[1].c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 2) {
			sscanf(args[2].c_str(), "%x", &eAddr);
			eAddr = sAddr + eAddr - 1;
		} else if (eAddr == -1)
			eAddr = sAddr + 0x140 - 1;
	}

	cpu = (vax_cpuDevice *)((sysDevice *)sdev)->getCPUDevice(0);

	cpu->dump(cty, &sAddr, eAddr);

	return CMD_OK;
}

// Usage: load <file> ...
static int cmdLoad(Console *con, Device *sdev, args_t &args)
{
	Device   *dev;
	uint32_t  sAddr = 0;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <file> [start]" << std::endl;
		return CMD_OK;
	}

	if (args.size() >= 3)
		sscanf(args[2].c_str(), "%x", &sAddr);

	sdev->load(args[1], sAddr);

	return CMD_OK;
}

// Usage: run <addr>
static int cmdRun(Console *con, Device *sdev, args_t &args)
{
	uint32_t       sAddr = 0;
	vax_cpuDevice *cpu;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " [addr]" << std::endl;
		return CMD_OK;
	}

	if (args.size() >= 2)
		sscanf(args[1].c_str(), "%x", &sAddr);

	// Get first CPU device from CPU device array
	cpu = (vax_cpuDevice *)((sysDevice *)sdev)->getCPUDevice(0);

	// Start execution
	cpu->reset();
	cpu->setPCAddr(sAddr);
	cpu->execute();

	return CMD_OK;
}

// General commands table
Command vaxCommands[] = {
	{ "boot", "<device> [options...]", cmdBoot },
	{ "disasm", "<start> [end]", cmdDisasm },
	{ "dump", "<start> [end]", cmdDump },
	{ "load", "<file> [start]", cmdLoad },
	{ "run", "<addr>", cmdRun },
	{ "start", "<device> [options...]", cmdStart },
	{ "stop", "<device>", cmdStop },
	// null terminator - end of command table
	{ nullptr }
};

// General set commands table
Command vaxSetCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};

// General show commands table
Command vaxShowCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};

// General list commands table
Command vaxListCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};



Driver vax_sysDriver {
	"VAX",
	"DEC VAX-11 Series",
	__FILE__,
	nullptr,

	// Configurations
	nullptr,

	// Command handlers
	vaxCommands,
	vaxSetCommands,
	vaxShowCommands,
	vaxListCommands,

	// Function calls
	nullptr
};


extern Driver ka655_sysDriver;
extern Driver ka655x_sysDriver;

sysModel vax_sysModels[] =
{
	SYSTEM(vax,   "VAX minicomputer",    &vax_sysDriver),

//	MODEL(vax730,   vax, "VAX-11/730",       ka730_sysDriver),
//	MODEL(vax750,   vax, "VAX-11/750",       ka750_sysDriver),
//	MODEL(vax780,   vax, "VAX-11/780",       ka780_sysDriver),
//	MODEL(vax782,   vax, "VAX-11/782",       ka780_sysDriver),
//	MODEL(vax785,   vax, "VAX-11/785",       ka780_sysDriver),

//	MODEL(vax8200,     vax, "VAX 8200",         ka820_sysDriver),
//	MODEL(vax8250,     vax, "VAX 8250",         ka820_sysDriver),
//	MODEL(vax8300,     vax, "VAX 8300",         ka820_sysDriver),
//	MODEL(vax8350,     vax, "VAX 8350",         ka820_sysDriver),
//	MODEL(vax8600,     vax, "VAX 8600",         ka86_sysDriver),
//	MODEL(vax8650,     vax, "VAX 8650",         ka86_sysDriver),

	// MicroVAX model list
//	MODEL(mv3300,   vax, "MicroVAX 3300",    ka640_sysDriver),
//	MODEL(mv3400,   vax, "MicroVAX 3400",    ka640_sysDriver),
//	MODEL(mv3500,   vax, "MicroVAX 3500",    ka650_sysDriver),
//	MODEL(mv3600,   vax, "MicroVAX 3600",    ka650_sysDriver),
	MODEL(mv3800,   vax, "MicroVAX 3800",    &ka655_sysDriver),
	MODEL(mv3800x,  vax, "MicroVAX 3800X",   &ka655x_sysDriver),
	MODEL(mv3900,   vax, "MicroVAX 3900",    &ka655_sysDriver),
	MODEL(mv3900x,  vax, "MicroVAX 3900X",   &ka655x_sysDriver),

	// VAXserver model list
//	MODEL(vs3300,   vax, "VAXserver 3300",   ka640_sysDriver),
//	MODEL(vs3400,   vax, "VAXserver 3400",   ka640_sysDriver),
//	MODEL(vs3500,   vax, "VAXserver 3500",   ka650_sysDriver),
//	MODEL(vs3600,   vax, "VAXserver 3600",   ka650_sysDriver),
	MODEL(vs3800,   vax, "VAXserver 3800",   &ka655_sysDriver),
	MODEL(vs3800x,  vax, "VAXserver 3800X",  &ka655x_sysDriver),
	MODEL(vs3900,   vax, "VAXserver 3900",   &ka655_sysDriver),
	MODEL(vs3900x,  vax, "VAXserver 3900X",  &ka655x_sysDriver),

	// VAXstation model list
//	MODEL(vs3200g,  vax, "VAXstation 3300",  ka640_sysDriver),
//	MODEL(vs3500g,  vax, "VAXstation 3500",  ka650_sysDriver),
//	MODEL(vs3520g,  vax, "VAXstation 3520",  ka60_sysDriver),
//	MODEL(vs3540g,  vax, "VAXstation 3540",  ka60_sysDriver),

	// null terminator
	{ nullptr }
};
