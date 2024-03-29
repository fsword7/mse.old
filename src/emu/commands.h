//
//  commands.hpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#pragma once

class command_handler;
class console_base;

enum cmdStatus {
	cmdOk = 0,
	cmdInvalid,
	cmdShutdown
};

typedef std::vector<std::string> args_t;
typedef cmdStatus (/* command_handler:: */ *cmdfunc_t)(console_base *, int, args_t &);

struct command_t {
	const char *name;
	cmdfunc_t   func;
};

class command_handler {
public:

	command_handler() {}


	cmdStatus execute(console_base *cty, std::string cmdLine);

protected:
	std::vector<std::string> split(std::string const &line);

	// commands table
//	cmdStatus cmdCreate(int argc, args_t &args);
//	cmdStatus cmdList(int argc, args_t &args);

	// List commands table
//	cmdStatus cmdListSystem(int argc, args_t &args);

	static command_t mseCommands[];
	static command_t mseSetCommands[];
	static command_t mseShowCommands[];
	static command_t mseListCommands[];
};
