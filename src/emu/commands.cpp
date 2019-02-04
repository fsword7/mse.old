/*
 * commands2.cpp
 *
 *  Created on: Jan 18, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/commands.h"
#include "emu/driver.h"
#include "emu/syslist.h"

static cmdStatus /* command_handler:: */ cmdCreate(int argc, args_t &args)
{
	system_list    sys;
	const system_driver *model;
	const system_config *config;


	if ((model = sys.find(args[2])) == nullptr) {
		std::cout << args[0] << ": System " << args[2] << " not found" << std::endl;
		return cmdOk;
	}
	config = new system_config(*model);

	return cmdOk;
}

static cmdStatus /* command_handler:: */ cmdExit(int argc, args_t &args)
{
	return cmdShutdown;
}

static cmdStatus /* command_handler:: */ cmdListSystem(int argc, args_t &args)
{
	system_list sys;

	std::cout << "System List" << std::endl;
	sys.list();

	return cmdOk;
}

// General commands table
command_t command_handler::mseCommands[] = {
	{ "create", cmdCreate },
	{ "exit", cmdExit },
	{ "quit", cmdExit },
	// null terminator - end of command table
	{ nullptr }
};

// General set commands table
command_t command_handler::mseSetCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};

// General show commands table
command_t command_handler::mseShowCommands[] = {
	// null terminator - end of command table
	{ nullptr }
};

// General list commands table
command_t command_handler::mseListCommands[] = {
	{ "system", cmdListSystem },
	// null terminator - end of command table
	{ nullptr }
};


