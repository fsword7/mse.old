/*
 * cmdhandler.cpp
 *
 *  Created on: Feb 4, 2019
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



