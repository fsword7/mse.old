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

const mapAddressConfig *di_memory::getAddressConfig(int space) const
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
