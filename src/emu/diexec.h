/*
 * diexec.h - Device Interface - Execution
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"

class di_execute : public device_interface
{
public:
	di_execute(device_t *dev);
	~di_execute();

	virtual void execute() = 0;
};
