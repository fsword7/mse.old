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
#define CMD_ERROR      3

class Console;
class Device;

typedef std::vector<std::string> args_t;

typedef int (*cmdfunc)(Console *, Device *, args_t &args);

struct Command {
	std::string name;
	std::string usage;
	cmdfunc execute;
};

class Console {
public:
	Console(Device *dev);
	~Console();

	void prompt();

	inline Device *getDevice() const { return root; }

private:
	Device *root; // root core device
};

int cmdCreate(Console *con, Device *dev, args_t &args);
int cmdExit(Console *con, Device *dev, args_t &args);

