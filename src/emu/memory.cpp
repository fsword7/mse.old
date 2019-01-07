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

// Address space type
#define AS_PROGRAM		0
#define AS_DATA			1
#define AS_IO			2

// Memory handler type
#define AMH_NONE		0
#define AMH_RAM			1
#define AMH_ROM			2

memManager::memManager(Device *sys)
{
}

memManager::~memManager()
{
}


memAddressEntry::memAddressEntry(Device &_dev, memAddressMap &_map, offs_t start, offs_t end)
: next(nullptr), map(_map), dev(_dev),
  adrStart(start), adrEnd(end),
  adrMask(0), adrMirror(0), adrSelect(0)
{

}

memAddressMap::memAddressMap(Device &_sys)
: sys(_sys)
{
}

memAddressMap::~memAddressMap()
{
}

memAddressEntry &memAddressMap::operator()(offs_t start, offs_t end)
{
	memAddressEntry *entry;

	entry = new memAddressEntry(sys, *this, start, end);
	list.push_back(*entry);

	return *entry;
}
