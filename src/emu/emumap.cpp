/*
 * emumap.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/addrmap.h"
#include "emu/dimem.h"
#include "emu/machine.h"


mapManager::mapManager(machine *sys)
: system(sys)
{
}

mapManager::~mapManager()
{
}

void mapManager::allocate(di_memory &memory)
{
	for (int space = 0; memory.mapConfigCount(); space++) {
		mapAddressConfig *config = memory.getAddressSpaceConfig(space);
		if (config != nullptr) {

		}
	}
}

void mapManager::init()
{
	dimem_iterator iter(*system->sysDevice());
	std::vector<di_memory *> memories;

	for (di_memory &memory : iter) {
		memories.push_back(&memory);
		allocate(memory);
	}
}

// ***********************************************************

mapAddressConfig::mapAddressConfig()
: name(nullptr), endianness(endianLittle),
  dataWidth(0), addrWidth(0),
  addrShift(0), pageShift(0),
  internalMap(mapConstructor()),
  defaultMap(mapConstructor())
{

}

mapAddressConfig::mapAddressConfig(tag_t *tag, endian_t endian,
		uint8_t dWidth, uint8_t aWidth, int8_t aShift,
		mapConstructor intmap, mapConstructor defmap)
: name(tag), endianness(endian),
  dataWidth(dWidth), addrWidth(aWidth),
  addrShift(aShift), pageShift(0),
  internalMap(intmap),
  defaultMap(defmap)
{

}

// ***********************************************************

void mapAddressSpace::prepare()
{
}

void mapAddressSpace::populate(mapAddress *map)
{
}

void mapAddressSpace::allocate()
{
}

void mapAddressSpace::locate()
{
}
