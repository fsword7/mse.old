/*
 * commands.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/devsys.h"

// Usage: create [device] <options...>
static int cmdCreate(Console *con, Device *cdev, args_t &args)
{
	sysModel   *model;
	Driver     *drv;
	std::string devType = "";
	Device     *ndev;

	// Check number of arguments
	if (args.size() < 3) {
		std::cout << "Usage: " << args[0] << " [device] [driver]" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	if (cdev->findDevice(args[1]) != nullptr) {
		std::cout << args[1] << ": device already created." << std::endl;
		return CMD_OK;
	}

	// find model by using name
	model = cdev->findModel(args[2]);
	if (model == nullptr) {
		std::cout << args[2] << ": system not found." << std::endl;
		return CMD_OK;
	}
	drv = model->driver;

	if (model->parent != nullptr) {
		devType += model->parent;
		devType += ".";
	}
	devType += model->name;

	ndev = drv->create(args[1], devType, model);
	cdev->addDevice(ndev);

	std::cout << devType << ": " << model->desc << std::endl;

	return CMD_OK;
}

// Usage: dial [device]
static int cmdDial(Console *con, Device *cdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <device|none>" << std::endl;
		return CMD_OK;
	}

	if (args[1] == "none") {
		dev = con->getDialedDevice();
		con->dialDevice(nullptr);
		if (dev != nullptr)
			std::cout << cdev->getName() << ": " << dev->getName()
			          << " released" << std::endl;
		else
			std::cout << cdev->getName() << ": already released" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = cdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

	con->dialDevice(dev);
	std::cout << cdev->getName() << ": Dialed " << dev->getName() << std::endl;

	return CMD_OK;
}

// Usage: dump <start> [end]
static int cmdDump(Console *con, Device *cdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <start> [end]" << std::endl;
		return CMD_OK;
	}

	// Check dialed device
	dev = con->getDialedDevice();
	if (dev == nullptr) {
		std::cerr << cdev->getName() << ": Please dial device first" << std::endl;
		return CMD_OK;
	}

	return CMD_OK;
}

// Usage: load <file> ...
static int cmdLoad(Console *con, Device *cdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <file> ..." << std::endl;
		return CMD_OK;
	}

	// Check dialed device
	dev = con->getDialedDevice();
	if (dev == nullptr) {
		std::cerr << cdev->getName() << ": Please dial device first" << std::endl;
		return CMD_OK;
	}

	dev->load(args[1]);

	return CMD_OK;
}

// Usage: set <device> <options...>
static int cmdSet(Console *con, Device *cdev, args_t &args)
{
	Device  *dev;
	const Command *cmds;
	int      rc;

	// Check number of arguments
	if (args.size() < 3) {
		std::cout << "Usage: " << args[0] << " <device> <options...>" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = cdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

	// Process commands by using console device
	cmds = cdev->getSetCommandTable();
	if (cmds != nullptr) {
		rc = CMD_NOTFOUND;
		for (int idx = 0; cmds[idx].name; idx++) {
			if (cmds[idx].name == args[2]) {
				rc = cmds[idx].execute(con, dev, args);
				break;
			}
		}
		if (rc == CMD_NOTFOUND) {
			std::cerr << cdev->getName() << ": Unknown option " << args[2] << std::endl;
			return CMD_OK;
		}
	} else {
		std::cerr << cdev->getName() << ": set command table not found." << std::endl;
		return CMD_OK;
	}

	return rc;
}

// Usage: set [device] memory [options...]
static int cmdSetMemory(Console *con, Device *cdev, args_t &args)
{
	cfgMemory *memcfg = cdev->getMemoryConfig();

	if (memcfg == nullptr) {
		std::cerr << cdev->getName() << ": Memory configuration table not found." << std::endl;
		return CMD_OK;
	}

	for (int idx = 0; memcfg[idx].cfgName; idx++) {
		if (memcfg[idx].cfgName == args[3]) {
			cdev->setMemory(memcfg[idx].cfgSize);
			std::cout << cdev->getName() << ": Set memory to "
					  << memcfg[idx].cfgName << std::endl;
			return CMD_OK;
		}
	}
	std::cerr << cdev->getName() << ": Config value not found." << std::endl;
	return CMD_OK;
}

// Usage: exit|quit
static int cmdShutdown(Console *, Device *, args_t &)
{
	// Shutdown system
	return CMD_SHUTDOWN;
}


// General commands table
Command mseCommands[] = {
	{ "create", "", cmdCreate },
	{ "dial", "<device|none>", cmdDial },
	{ "dump", "<start> [end]", cmdDump },
	{ "load", "<file> ...", cmdLoad },
	{ "exit", "", cmdShutdown },
	{ "quit", "", cmdShutdown },
	{ "set", "set [device] <options...>", cmdSet },
	// null terminator - end of command table
	{ nullptr }
};

// General set commands table
Command mseSetCommands[] = {
	{ "mem", "set [device] mem <options...>", cmdSetMemory },
	// null terminator - end of command table
	{ nullptr }
};

// General show commands table
Command mseShowCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};

// General list commands table
Command mseListCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};
