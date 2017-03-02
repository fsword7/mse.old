/*
 * console.h
 *
 *  Created on: Mar 2, 2017
 *      Author: fswor
 */

#pragma once

// Return code (error codes)
#define CMD_OK         0
#define CMD_SHUTDOWN   1
#define CMD_NOTFOUND   2

class System;
class Console;

typedef std::vector<std::string> args_t;

typedef int (*cmdfunc)(Console *, args_t &args);

struct Command {
	std::string name;
	std::string usage;
	cmdfunc execute;
};

class Console {
public:
	Console(System *sys);
	~Console();

	void prompt();

	inline System *getSystem() const { return sys; }

private:
	System *sys;
};

int cmdCreate(Console *con, args_t &args);
int cmdExit(Console *con, args_t &args);

