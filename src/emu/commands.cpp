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

static cmdStatus /* command handler:: */ cmdDisasm(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.disassemble(argc, args);

	return cmdOk;
}

static cmdStatus /* command handler:: */ cmdDump(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.dump(argc, args);

	return cmdOk;
}

static cmdStatus /* command handler:: */ cmdDumpr(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.dumpr(argc, args);

	return cmdOk;
}

static cmdStatus /* command_handler:: */ cmdExit(cty_t *cty, int argc, args_t &args)
{
	return cmdShutdown;
}

static cmdStatus /* command handler:: */ cmdStart(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	engine.start(argc, args);

	return cmdOk;
}

static cmdStatus /* command_handler:: */ cmdListDevices(cty_t *cty, int argc, args_t &args)
{
	system_engine engine(*cty);

	std::cout << "System List" << std::endl;
	engine.listDevices(argc, args);

	return cmdOk;
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
	{ "disasm", cmdDisasm },
	{ "dump", cmdDump },
	{ "dumpr", cmdDumpr },
	{ "exit", cmdExit },
	{ "quit", cmdExit },
	{ "start", cmdStart },
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
	{ "devices", cmdListDevices },
	{ "system", cmdListSystem },
	{ "machine", cmdListMachine },
	// null terminator - end of command table
	{ nullptr }
};


