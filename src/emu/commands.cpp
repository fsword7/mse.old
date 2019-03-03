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
#include "emu/engine.h"

static cmdStatus /* command_handler:: */ cmdCreate(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.create(args[1], args[2]);

	return cmdOk;
}

static cmdStatus /* command handler:: */ cmdDump(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.dump(argc, args);

	return cmdOk;
}

static cmdStatus /* command_handler:: */ cmdExit(cty_t *cty, int argc, args_t &args)
{
	return cmdShutdown;
}

static cmdStatus /* command_handler:: */ cmdListSystem(cty_t *cty, int argc, args_t &args)
{
	system_list sys;

	std::cout << "System List" << std::endl;
	sys.list();

	return cmdOk;
}

static cmdStatus /* commander_handler:: */ cmdListMachine(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.list();

	return cmdOk;
}

// General commands table
command_t command_handler::mseCommands[] = {
	{ "create", cmdCreate },
	{ "dump", cmdDump },
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
	{ "machine", cmdListMachine },
	// null terminator - end of command table
	{ nullptr }
};


