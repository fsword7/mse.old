/*
 * console.h
 *
 *  Created on: Mar 2, 2017
 *      Author: fswor
 */

#pragma once


class Device;

class Console {
public:
	Console(Device *dev);
	~Console();

	int  executeCommand(args_t &args);
	void prompt();
	void script(std::string fname);

	inline Device *getDevice() const { return cdev; }
	inline Device *getSystemDevice() const { return sdev; }

	void setSystemDevice(Device *dev)  { sdev = dev; }

private:
	Device *cdev; // Console device
	Device *sdev; // Current system device
};


