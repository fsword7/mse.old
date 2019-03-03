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

#include "emu/map/hedr.h"
#include "emu/map/hedw.h"

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
  addrMask(makeBitMask<offs_t>(config.address_width())),
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

void mapAddressSpace::prepare(const cty_t &cty)
{
	mapMemoryRegion *devRegion = (space == 0) ? device.mapGetMemoryRegion("") : nullptr;
	uint32_t devRegionSize = (devRegion != nullptr) ? devRegion->size() : 0;

	map = new mapAddress(device, space);

	cty.printf("%s: Preparing for maps...\n", device.tagName());

	// Merge with submaps
	map->importSubmaps(*manager.sysMachine(), device.owner() ? *device.owner() : device, data_width(), endian());

	unmapValue = (map->unmapValue == 0) ? 0 : ~0;
	if (map->gmask != 0) {
		if (map->gmask & ~addrMask)
			cty.printf("%s: Can't set a global mask of %08X on a %d-bit address width bus (mask %08X).\n",
				device.tagName(), map->gmask, addr_width(), addrMask);
		addrMask = map->gmask;
	}

	for (mapAddressEntry *entry : map->list) {

		cty.printf("%s: %s space - %08X-%08X mask %08X mirror %08X\n", device.tagName(), name,
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
				cty.printf("%s: %s space - %08X-%08X - nonexistent region '%s'\n",
					device.tagName(), name, entry->adrStart, entry->adrEnd, entry->tagRegion);

			if ((entry->rgnOffset + config.address_to_byte(entry->adrEnd - entry->adrStart + 1)) > region->size())
				cty.printf("%s: %s space - %08X-%08X extends beyond region '%s'\n",
					device.tagName(), name, entry->adrStart, entry->adrEnd, entry->tagRegion);

			// Now assign named region to memory pointers
			entry->memory = region;
		}

	}
}

void mapAddressSpace::populateEntry(const cty_t &cty, const mapAddressEntry &entry, rwType type)
{

	const mapHandler &data = (type == rwType::READ) ? entry.read : entry.write;

	switch (data.type) {
	case mapNone:
		return;

	case mapROM:
		if (type == rwType::WRITE)
			return;
	case mapRAM:
		setup_ram_generic(cty, entry.adrStart, entry.adrEnd, entry.adrMirror, type, nullptr);
		break;

	case mapNop:
		setup_unmap_generic(cty, entry.adrStart, entry.adrEnd, entry.adrMirror, type, true);
		break;

	case mapUnmapped:
		setup_unmap_generic(cty, entry.adrStart, entry.adrEnd, entry.adrMirror, type, false);
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
		cty.printf("Internal mapping error: leftover mapping of %s\n", data.tag);
		break;
	}
}

void mapAddressSpace::populate(const cty_t &cty, mapAddress *map)
{
	if (map == nullptr)
		map = this->map;
	if (map == nullptr)
		return;

	cty.printf("%s: Populating from maps...\n", device.tagName());

	for (const mapAddressEntry *entry : map->list) {
		populateEntry(cty, *entry, rwType::READ);
		populateEntry(cty, *entry, rwType::WRITE);
	}


}

void mapAddressSpace::allocate(const cty_t &cty)
{
}

void mapAddressSpace::locate(const cty_t &cty)
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

void mapMemoryManager::init(const cty_t &cty)
{
	dimem_iterator iter(*system->getSystemDevice());
	std::vector<di_memory *> memories;

	for (di_memory &memory : iter) {
//		std::cout << "Device " << memory.getDevice()->deviceName() << std::endl;
		memories.push_back(&memory);
		allocate(cty, memory);
	}


	for (auto const memory : memories)
		memory->prepare(cty);

	for (auto const memory : memories)
		memory->populate(cty);

	for (auto const memory : memories)
		memory->allocate(cty);

	for (auto const memory : memories)
		memory->locate(cty);

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

mapMemoryBlock::mapMemoryBlock(mapAddressSpace &space, offs_t start, offs_t end, void *memory)
: system(*space.getManager().sysMachine()),
  addrSpace(space),
  addrStart(start),
  addrEnd(end),
  baseData(reinterpret_cast<uint8_t *>(memory))
{
	const offs_t length = space.address_to_byte(end + 1 - start);

	// Initialize memory block if not available
	if (baseData == nullptr) {
		allocated.resize(length);
		memset(&allocated[0], 0, length);
		baseData = &allocated[0];
	}
}
