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

typedef device_interface_iterator<di_memory> dimem_iterator;

class di_memory : public device_interface
{
public:
	di_memory(device_t *dev);
	~di_memory();

	mapConstructor getAddressMap(int space = 0);
	const mapAddressConfig *getAddressSpaceConfig(int space) const;
	mapAddressConfig *getAddressSpaceConfig(int space);

	int mapConfigCount() { return mapConfig.size(); }

	void setAddressMap(int space, mapConstructor map);

	void validate() const;

protected:
	std::vector<mapConstructor>		mapAddress;

private:
	std::vector<mapAddressConfig *>	mapConfig;
	std::vector<mapAddressSpace>	mapSpace;
};
