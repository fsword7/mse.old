/*
 * dimem.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"

class di_memory : public device_interface
{
public:
	di_memory(device_t *dev);
	~di_memory();
};
