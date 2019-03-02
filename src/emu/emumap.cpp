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
#include "emu/exception.h"

#include "emu/emumap_hedr.h"
#include "emu/emumap_hedw.h"

// ***********************************************************

mapAddressConfig::mapAddressConfig()
: name(nullptr), endianness(endianLittle),
  dataWidth(0), addrWidth(0),
  addrShift(0), pageShift(0),
  internalMap(mapConstructor())
{
}

mapAddressConfig::mapAddressConfig(tag_t *tag, endian_t endian,
		uint8_t dWidth, uint8_t aWidth, int8_t aShift,
		mapConstructor intmap)
: name(tag), endianness(endian),
  dataWidth(dWidth), addrWidth(aWidth),
  addrShift(aShift), pageShift(0),
  internalMap(intmap)
{
}

// ***********************************************************

mapAddressSpace::mapAddressSpace(mapMemoryManager &manager, di_memory &memory, int space)
: config(*memory.getAddressSpaceConfig(space)),
  name(config.getName()),
  space(space),
  device(*memory.getDevice()),
  manager(manager),
  map(nullptr),
  unmapValue(0),
  readUnmap(nullptr), writeUnmap(nullptr),
  readNop(nullptr), writeNop(nullptr)
{

}

mapAddressSpace::~mapAddressSpace()
{
}

inline void mapAddressSpace::adjustAddresses(offs_t &start, offs_t &end, offs_t &mask, offs_t &mirror)
{
	mask  &= addrMask;
	start &= ~mirror & addrMask;
	end   &= ~mirror & addrMask;
}

void mapAddressSpace::prepare()
{
	mapMemoryRegion *devRegion = (space == 0) ? device.mapGetMemoryRegion("") : nullptr;
	uint32_t devRegionSize = (devRegion != nullptr) ? devRegion->size() : 0;

	map = new mapAddress(device, space);

	// Merge with submaps
	map->importSubmaps(*manager.sysMachine(), device.owner() ? *device.owner() : device, data_width(), endian());

	unmapValue = (map->unmapValue == 0) ? 0 : ~0;
	if (map->gmask != 0) {
//		if (map->gmask & ~addrMask)
//			mseFatalError("Can't set a global mask of %08X on a %d-bit address width bus.\n",
//				map->gmask, address_width());
		addrMask = map->gmask;
	}

	for (mapAddressEntry *entry : map->list) {

		msePrintf("%s: %s space memory entry %08X-%08X mask %08X mirror %08X\n", device.tagName(), name,
			entry->adrStart, entry->adrEnd, entry->adrMask, entry->adrMirror);

		// Adjust addresses first
		adjustAddresses(entry->adrStart, entry->adrEnd, entry->adrMask, entry->adrMirror);

		if (entry->tagShare != nullptr) {

		}

		if (space == 0 && entry->read.type == mapROM && entry->tagRegion == nullptr) {
			if (entry->adrEnd < devRegionSize) {
				entry->tagRegion = device.tagName();
				entry->rgnOffset = config.address_to_byte(entry->adrStart);
			}
		}

		// Validate adjusted addresses against implicit regions
		// and assign region to memory pointers
		if (entry->tagRegion != nullptr) {
			mapMemoryRegion *region = manager.sysMachine()->getSystemDevice()->mapGetMemoryRegion(entry->tagRegion);

			if (region == nullptr)
				msePrintf("Device '%s' %s space memory entry %X-%X - nonexistent region '%s'\n",
					device.tagName(), name, entry->adrStart, entry->adrEnd, entry->tagRegion);

			if ((entry->rgnOffset + config.address_to_byte(entry->adrEnd - entry->adrStart + 1)) > region->size())
				msePrintf("Device '%s' %s space memory entry %X-%X extends beyond region '%s'\n",
					device.tagName(), name, entry->adrStart, entry->adrEnd, entry->tagRegion);

			// Now assign named region to memory pointers
			entry->memory = region;
		}

	}
}

void mapAddressSpace::populateEntry(const mapAddressEntry &entry, rwType type)
{

	const mapHandler &data = (type == rwType::READ) ? entry.read : entry.write;

	switch (data.type) {
	case mapNone:
		return;

	case mapROM:
		if (type == rwType::WRITE)
			return;
	case mapRAM:
		setup_ram_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, nullptr);
		break;

	case mapNop:
		setup_unmap_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, true);
		break;

	case mapUnmapped:
		setup_unmap_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, false);
		break;

	case mapPort:
//		setup_rw_port(entry.adrStart, entry.adrEnd, entry.adrMirror,
//				(type == rwType::READ) ? "" : ""),
//				(type == rwType::WRITE) ? "" : ""));
		break;

	case mapBank:
//		setup_bank_port(entry.adrStart, entry.adrEnd, entry.adrMirror,
//				(type == rwType::READ) ? "" : ""),
//				(type == rwType::WRITE) ? "" : ""));
		break;

	case mapSubmap:
		msePrintf("Internal mapping error: leftover mapping of %s\n", data.tag);
		break;
	}
}

void mapAddressSpace::populate(mapAddress *map)
{
	if (map == nullptr)
		map = this->map;
	if (map == nullptr)
		return;

	for (const mapAddressEntry *entry : map->list) {
		populateEntry(*entry, rwType::READ);
		populateEntry(*entry, rwType::WRITE);
	}


}

void mapAddressSpace::allocate()
{
}

void mapAddressSpace::locate()
{
}

// ******************************************************************

mapMemoryManager::mapMemoryManager(machine *sys)
: system(sys)
{
}

mapMemoryManager::~mapMemoryManager()
{
}

void mapMemoryManager::init()
{
	dimem_iterator iter(*system->getSystemDevice());
	std::vector<di_memory *> memories;

	for (di_memory &memory : iter) {
//		std::cout << "Device " << memory.getDevice()->deviceName() << std::endl;
		memories.push_back(&memory);
		allocate(memory);
	}


	for (auto const memory : memories)
		memory->prepare();

	for (auto const memory : memories)
		memory->populate();

	for (auto const memory : memories)
		memory->allocate();

	for (auto const memory : memories)
		memory->locate();

}

mapMemoryRegion *mapMemoryManager::allocateRegion(tag_t *name, uint32_t length, uint8_t width, endian_t endian)
{
	mapMemoryRegion *region;


	region = new mapMemoryRegion(system, name, length, width, endian);
	regionList.emplace(name, region);

	return region;
}

void mapMemoryManager::freeRegion(tag_t *name)
{
	regionList.erase(name);
}
