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

class Console;

typedef int (*cmdfunc)();

struct Command {
	std::string name;
	std::string usage;
	cmdfunc execute;
};


class Console {
public:
	Console();
	~Console();

	void prompt();

private:
};

