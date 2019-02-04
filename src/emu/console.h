/*
 * console.h
 *
 *  Created on: Mar 2, 2017
 *      Author: fswor
 */

#pragma once

#include "emu/commands.h"

class console_base {
public:
	console_base();
	~console_base();

	void printf(const char *format, ...);

	void prompt();
	void script(std::string fname);

//	inline Device *getDevice() const { return cdev; }
//	inline Device *getSystemDevice() const { return sdev; }
//
//	void setSystemDevice(Device *dev)  { sdev = dev; }


private:
//	Device *cdev; // Console device
//	Device *sdev; // Current system device
};

using cty_t = console_base;
