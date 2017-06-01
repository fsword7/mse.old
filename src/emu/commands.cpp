/*
 * commands.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/console.h"

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

// Usage: debug <device> <option|all> <on|off>
//        debug <device> log <slot|all|cty> <on|off>
static int cmdDebug(Console *con, Device *cdev, args_t &args)
{
	Device *sdev, *dev;
	Debug  *dbg;
	bool    onFlag = false;
	int     slot = 0;
	int     ok;

	// Check number of arguments
	if (args.size() < 3) {
		std::cout << "Usage: " << args[0] << " <device> <option|all> <on|off>" << std::endl;
		std::cout << "       " << args[0] << " <device> log <slot|all|cty> <on|off>" << std::endl;
		return CMD_OK;
	}

	// Check current system device
	sdev = con->getSystemDevice();
	if (sdev == nullptr) {
		std::cerr << cdev->getName() << ": Please select system device first" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = sdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}


	dbg = dev->getDebug();
	if (boost::iequals(args[2], "log")) {
		// Parse on/off argument
		if (boost::iequals(args[4], "on"))
			onFlag = true;
		else if (boost::iequals(args[4], "off"))
			onFlag = false;
		else {
			std::cout << "Invalid on/off option" << std::endl;
			return CMD_OK;
		}

		// Parse cty, all or slot argument
		if (boost::iequals(args[3], "cty")) {
			if (onFlag == true)
				dbg->enableLog(LOG_CTYSLOT);
			else
				dbg->disableLog(LOG_CTYSLOT);
		} else if (boost::iequals(args[3], "all")) {
			if (onFlag == true)
				dbg->enableLog(LOG_ALLSLOTS);
			else
				dbg->disableLog(LOG_ALLSLOTS);
		} else {
			if (sscanf(args[3].c_str(), "%d", &slot) != 1) {
				std::cout << "Invalid slot argument" << std::endl;
				return CMD_OK;
			} else if ((slot < 0) || (slot > LOG_NFILES)) {
				std::cout << "Out of valid range (0 - " << LOG_NFILES << ")" << std::endl;
				return CMD_OK;
			}
			if (onFlag == true)
				dbg->enableLog(slot);
			else
				dbg->disableLog(slot);
		}
	} else  {
		// Parse on/off argument
		if (boost::iequals(args[3], "on"))
			onFlag = true;
		else if (boost::iequals(args[3], "off"))
			onFlag = false;
		else {
			std::cout << "Invalid on/off option" << std::endl;
			return CMD_OK;
		}

		// Parse debug option argument
		if (onFlag == true)
			ok = dbg->enableFlag(args[2]);
		else
			ok = dbg->disableFlag(args[2]);
		if (ok == CMD_NOTFOUND) {
			std::cout << "Invalid debug option: " << args[2] << std::endl;
			return CMD_OK;
		}
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

	// Check current system device
	dev = con->getSystemDevice();
	if (dev == nullptr) {
		std::cerr << cdev->getName() << ": Please select system device first" << std::endl;
		return CMD_OK;
	}

	dev->load(args[1], 0);

	return CMD_OK;
}

// Usage: log <file>
static int cmdLog(Console *con, Device *cdev, args_t &args)
{
	sysDevice *sdev;
	logFile   *log;
	int        slot = 0;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <file> [slot]" << std::endl;
		return CMD_OK;
	}

	// Check current system device
	sdev = (sysDevice *)con->getSystemDevice();
	if (sdev == nullptr) {
		std::cerr << cdev->getName() << ": Please select system device first" << std::endl;
		return CMD_OK;
	}

	// Get slot number
	if (args.size() > 2) {
		sscanf(args[2].c_str(), "%d", &slot);
		if (slot < 0 || slot >= LOG_NFILES) {
			std::cerr << cdev->getName() << ": Please select slot 0 to " << LOG_NFILES-1 << std::endl;
			return CMD_OK;
		}
	}

	log = sdev->getLogFile();
	log->open(args[1], slot);

	return CMD_OK;
}

// Usage: select <device|none>
static int cmdSelect(Console *con, Device *cdev, args_t &args)
{
	Device *dev;

	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <device|none>" << std::endl;
		return CMD_OK;
	}

	if (args[1] == "none") {
		dev = con->getSystemDevice();
		con->setSystemDevice(nullptr);
		if (dev != nullptr)
			std::cout << cdev->getName() << ": Device " << dev->getName()
			          << " released" << std::endl;
		else
			std::cout << cdev->getName() << ": device already released" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	dev = cdev->findDevice(args[1]);
	if (dev == nullptr) {
		std::cout << args[1] << ": device not found." << std::endl;
		return CMD_OK;
	}

	con->setSystemDevice(dev);
	std::cout << cdev->getName() << ": Selected device " << dev->getName() << std::endl;

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
	{ "debug", "", cmdDebug },
	{ "load", "<file> ...", cmdLoad },
	{ "log", "<file>", cmdLog },
	{ "exit", "", cmdShutdown },
	{ "quit", "", cmdShutdown },
	{ "select", "<device|none>", cmdSelect },
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
