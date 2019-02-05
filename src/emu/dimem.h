/*
 * dimem.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"
#include "emu/addrmap.h"
#include "emu/emumap.h"

class di_memory : public device_interface
{
public:
	di_memory(device_t *dev);
	~di_memory();

protected:
	std::vector<mapConstructor>		mapAddress;

private:
	std::vector<mapAddressConfig *>	mapConfig;
	std::vector<mapAddressSpace>	mapSpace;
};
