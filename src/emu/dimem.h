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

#define cfgSetAddressMap(_cpu, _space, _map) \
	dynamic_cast<di_memory *>(_cpu)->setAddressMap(_space, \
		mapConstructor(&std::remove_pointer_t<decltype(this)>::_map, tag().c_str(), this))

//#define cfgSetAddressMap(_cpu, _space, _map) \
//	dynamic_cast<di_memory *>(_cpu)->setAddressMap(_space, \
//		mapConstructor(&_map, tag().c_str(), this))

class di_memory : public device_interface
{
public:
	di_memory(device_t *dev);
	~di_memory();

	mapConstructor getAddressMap(int space = 0);
	const mapAddressConfig *getAddressConfig(int space) const;

	int mapConfigCount() { return mapConfig.size(); }

	void setAddressMap(int space, mapConstructor map);

protected:
	std::vector<mapConstructor>		mapAddress;

private:
	std::vector<mapAddressConfig *>	mapConfig;
	std::vector<mapAddressSpace>	mapSpace;
};
