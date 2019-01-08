/*
 * memory.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/memory.h"


memManager::memManager(Device *sys)
{
}

memManager::~memManager()
{
}


mapAddressEntry::mapAddressEntry(Device &_dev, mapAddress &_map, offs_t start, offs_t end)
: mapNext(nullptr), map(_map), dev(_dev),
  adrStart(start), adrEnd(end),
  adrMask(0), adrMirror(0), adrSelect(0),
  tagShare(nullptr)
{

}

mapAddressEntry &mapAddressEntry::mask(offs_t mask)
{
	adrMask = mask;
	if (map.gmask != 0)
		adrMask &= map.gmask;
	return *this;
}

mapAddress::mapAddress(Device &_sys)
: sys(_sys)
{
}

mapAddress::~mapAddress()
{
}

mapAddressEntry &mapAddress::operator()(offs_t start, offs_t end)
{
	mapAddressEntry *entry;

	entry = new mapAddressEntry(sys, *this, start, end);
	list.push_back(*entry);

	return *entry;
}
