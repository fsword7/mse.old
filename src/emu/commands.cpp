/*
 * commands.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: Timothy Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"

// Usage: create [device] <options...>
static int cmdCreate(Console *con, Device *dev, args_t &args)
{
	Driver *drv;

	// Check number of arguments
	if (args.size() < 3) {
		std::cout << "Usage: " << args[0] << " [device] [driver]" << std::endl;
		return CMD_OK;
	}

	// check existing device by using name
	if (dev->findDevice(args[1]) != nullptr) {
		std::cout << args[1] << ": name already taken or device not found." << std::endl;
		return CMD_OK;
	}

	// find available driver by using name
	drv = dev->findDriver(args[2]);
	if (drv == nullptr) {
		std::cout << args[2] << ": driver not found." << std::endl;
		return CMD_OK;
	}

	return CMD_OK;
}

// Usage: load <file> ...
static int cmdLoad(Console *con, Device *dev, args_t &args)
{
	// Check number of arguments
	if (args.size() < 2) {
		std::cout << "Usage: " << args[0] << " <file> ..." << std::endl;
		return CMD_OK;
	}

	dev->load(args[1]);

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
	{ "load", "<file> ...", cmdLoad },
	{ "exit", "", cmdShutdown },
	{ "quit", "", cmdShutdown },
	// end of command table
	{ nullptr }
};
