/*
 * dimem.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/dimem.h"

di_memory::di_memory(device_t *dev)
: device_interface(dev, "memory")
{
	mapAddress.clear();
	mapConfig.clear();
	mapSpace.clear();

	dev->interfaces().memory = this;
}

di_memory::~di_memory()
{
}

mapConstructor di_memory::getAddressMap(int space)
{
	if (space >= 0 && space < mapAddress.size())
		return mapAddress[space];
	else
		return mapConstructor();
}

mapAddressConfig *di_memory::getAddressSpaceConfig(int space)
{
	if (space >= 0 && space < mapConfig.size())
		return mapConfig[space];
	return nullptr;
}

const mapAddressConfig *di_memory::getAddressSpaceConfig(int space) const
{
	if (space >= 0 && space < mapConfig.size())
		return mapConfig[space];
	return nullptr;
}

void di_memory::setAddressMap(int space, mapConstructor map)
{
	if (space >= mapAddress.size())
		mapAddress.resize(space+1);
	mapAddress[space] = map;
}

void di_memory::validate() const
{
	const int maxSpaces = std::max(mapAddress.size(), mapConfig.size());

	for (int space = 0; space < maxSpaces; space++) {
		if (getAddressSpaceConfig(space)) {
			// Construct the address space mapping
//			::mapAddress addrmap(const_cast<device_t &>(device()), space);
//
//			addrmap.validate(space);
		}
	}
}
