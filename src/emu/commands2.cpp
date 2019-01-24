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

std::vector<std::string> command_handler::split(std::string const &line)
{
    std::istringstream buffer(line);
    std::vector<std::string> ret;

    std::copy(std::istream_iterator<std::string>(buffer),
              std::istream_iterator<std::string>(),
              std::back_inserter(ret));
    return ret;
}

cmdStatus command_handler::execute(std::string cmdLine)
{
	args_t		args;
	int			argc = 0;
	command_t	*cmdList;

	args = split(cmdLine);
	if (args.size() == 0)
		return cmdOk;

	if (args[0] == "set") {
		cmdList = mseSetCommands;
		argc++;
	} else if (args[0] == "show") {
		cmdList = mseShowCommands;
		argc++;
	} else if (args[0] == "list") {
		cmdList = mseListCommands;
		argc++;
	} else
		cmdList = mseCommands;

	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args[argc])
			return cmdList[idx].func(argc, args);
	}

	// Not found - invalid command
	return cmdInvalid;
}

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


