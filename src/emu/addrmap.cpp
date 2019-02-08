/*
 * addrmap.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/dimem.h"
#include "emu/addrmap.h"
#include "emu/exception.h"


mapAddressEntry::mapAddressEntry(device_t &_dev, mapAddress &_map, offs_t start, offs_t end)
: mapNext(nullptr), map(_map), device(_dev),
  adrStart(start), adrEnd(end),
  adrMask(0), adrMirror(0), adrSelect(0),
  tagShare(nullptr), submapDevice(nullptr)
{

}

mapAddressEntry &mapAddressEntry::mask(offs_t mask)
{
	adrMask = mask;
	if (map.gmask != 0)
		adrMask &= map.gmask;
	return *this;
}

mapAddress::mapAddress(device_t &_dev, int space)
: device(_dev),
  adrSpace(space)
{
	di_memory			*memory;
	const mapAddressConfig	*config;

	if (!device.hasInterface(memory))
		throw mseFatalError("No memory interface for device '%s'\n",
			device.tag());

	config = memory->getAddressSpaceConfig(adrSpace);
	if (config == nullptr)
		throw mseFatalError("No memory address space configuration for device '%s'\n",
			device.tag());

	if (memory->getAddressMap(adrSpace).isnull())
	{
		memory->getAddressMap(adrSpace)(*this);
	} else {
		if (!config->defaultMap.isnull())
			config->defaultMap(*this);
	}

	if (!config->internalMap.isnull())
		config->internalMap(*this);
}

mapAddress::mapAddress(device_t &_dev, mapAddressEntry *entry)
: device(_dev),
  adrSpace(AS_PROGRAM)
{
	entry->submapDelegate.bind(_dev);
	entry->submapDelegate(*this);
}


mapAddress::~mapAddress()
{
}

mapAddressEntry &mapAddress::operator()(offs_t start, offs_t end)
{
	mapAddressEntry *entry;

	entry = new mapAddressEntry(device, *this, start, end);
	list.push_back(*entry);

	return *entry;
}
