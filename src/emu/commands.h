//
//  commands.hpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#pragma once

class command_handler;

enum cmdStatus {
	cmdOk = 0,
};

typedef std::vector<std::string> args_t;
typedef cmdStatus (/* command_handler:: */ *cmdfunc_t)(int, args_t &);

struct command_t {
	const char *name;
	cmdfunc_t   func;
};

class command_handler {
public:

	command_handler() {}


	void execute(std::string cmdLine);

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
