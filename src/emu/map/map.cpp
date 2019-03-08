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

void mapAddressSpace::adjustAddresses(offs_t &start, offs_t &end, offs_t &mask, offs_t &mirror)
{
	mask  &= addrMask;
	start &= ~mirror & addrMask;
	end   &= ~mirror & addrMask;
}

void mapAddressSpace::checkOptimizeMirror(const cty_t &cty, tag_t *func,
		offs_t adrStart, offs_t adrEnd, offs_t adrMirror,
		offs_t &nstart, offs_t &nend, offs_t &nmask, offs_t &nmirror)
{
	if (adrStart > adrEnd)
		cty.printf("%s: (%s) In range %X-%X mirror %X, start address is after end address\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror);
	if (adrStart & ~addrMask)
		cty.printf("%s: (%s) In range %X-%X mirror %X, start address is outside of global address mask %X, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, addrMask, adrStart & addrMask);
	if (adrEnd & ~addrMask)
		cty.printf("%s: (%s) In range %X-%X mirror %X, end address is outside of global address mask %X, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, addrMask, adrEnd & addrMask);

	offs_t lowMask = (config.address_width() >> (3 - config.address_shift())) - 1;

	if (adrStart & lowMask)
		cty.printf("%s: (%s) In range %X-%X mirror %X, start address has low bits set, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, adrStart & ~lowMask);
	if ((~adrEnd) & lowMask)
		cty.printf("%s: (%s) In range %X-%X mirror %X, end address has low bits unset, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, adrEnd | lowMask);


	offs_t setBits = adrStart | adrEnd;
	offs_t maskBits = adrStart ^ adrEnd;

	if (adrMirror & ~addrMask)
		cty.printf("%s: (%s) In range %X-%X mirror %X, mirror address is outside of global address mask %X, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, addrMask, adrMirror & addrMask);
	if (adrMirror & maskBits)
		cty.printf("%s: (%s) In range %X-%X mirror %X, mirror address touches a changing address bit, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, adrMirror & ~maskBits);
	if (adrMirror & setBits)
		cty.printf("%s: (%s) In range %X-%X mirror %X, mirror address touches a set address bit, did you mean %X?\n",
			device.tagName(), func, adrStart, adrEnd, adrMirror, adrMirror & ~setBits);

	nstart  = adrStart;
	nend    = adrEnd;
	nmask   = maskBits;
	nmirror = adrMirror;

//	if (nmirror && !(nstart & chgBits) && !((~nend) & chgBits)) {
//		while (nmirror & (chgBits + 1)) {
//			offs_t bit = nmirror & (chgBits+1));
//			nmirror &= ~bit;
//			nend |= bit;
//			chgBits |= bit;
//		}
//	}
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

	case mapBank:
		setup_bank_generic(cty, entry.adrStart, entry.adrEnd, entry.adrMirror,
				((type == rwType::READ) ? device.tagName() : ""),
				((type == rwType::WRITE) ? device.tagName() : ""));
		break;

	case mapPort:
		setup_rw_port(cty, entry.adrStart, entry.adrEnd, entry.adrMirror,
				((type == rwType::READ) ? device.tagName() : ""),
				((type == rwType::WRITE) ? device.tagName() : ""));
		break;

	case mapDelegate:
//		if (type == rwType::READ) {
//			switch (data.bits) {
//			case 8:
//				setup_read_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					read8_delegate(entry.read8, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 16:
//				setup_read_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					read16_delegate(entry.read16, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 32:
//				setup_read_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					read32_delegate(entry.read32, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 64:
//				setup_read_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					read64_delegate(entry.read64, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			}
//		} else {
//			switch (data.bits) {
//			case 8:
//				setup_write_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					write8_delegate(entry.write8, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 16:
//				setup_write_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					write16_delegate(entry.write16, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 32:
//				setup_write_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					write32_delegate(entry.write32, entry.device), entry.laneMask, entry.csWidth);
//				break;
//			case 64:
//				setup_write_handler(entry.adrStart, entry.adrEnd, entry.adrMask, entry.adrMirror, entry.adrSelect,
//					write64_delegate(entry.write64, entry.device), entry.laneMask, entry.csWidth);
//				break;
//
//			}
//		}
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
	cty.printf("%s: Allocating memory space...\n", device.tagName());

	auto &blockList = manager.blocks();

	int tail = blockList.size();
	for (mapAddressEntry *entry : map->list)
		if (entry->memory != nullptr)
			blockList.push_back(new mapMemoryBlock(*this, entry->adrStart, entry->adrEnd, entry->memory));

//	mapAddressEntry *unassigned = nullptr;
//
//	for (mapMemoryBlock *block = blockList[tail]; tail != blockList.size(); ++tail)
//		unassigned = assignBlockIntersecting(cty, block->start(), block->end(), block->base());
//
//	if (unassigned == nullptr)
//		unassigned = assignBlockIntersecting(cty, ~0, 0, nullptr);
//
//
}

void mapAddressSpace::locate(const cty_t &cty)
{
	cty.printf("%s: Locating memory space...\n", device.tagName());
}

bool mapAddressSpace::needBackingMemory(mapAddressEntry &entry)
{
//	if (entry.share != nullptr) {
//
//	}

	if (entry.write.type == mapBank || entry.write.type == mapRAM)
		return true;

	mapMemoryRegion *region = manager.sysMachine()->getSystemDevice()->mapGetMemoryRegion(device.tagName());
	if (entry.read.type == mapRAM || (entry.read.type == mapROM &&
			(space != 0 || region == nullptr || entry.adrStart >= region->size())))
		return true;

	return false;
}

void *mapAddressSpace::findBackingMemory(const cty_t &cty, offs_t adrStart, offs_t adrEnd)
{
	uint8_t *result = nullptr;

	cty.printf("%s(%s): Finding blacking memory (%08X-%08X)\n", device.tagName(), name, adrStart, adrEnd);

	if (map == nullptr)
		return nullptr;

	for (mapAddressEntry *entry : map->list)
		if (entry->memory != nullptr && adrStart >= entry->adrStart && adrEnd <= entry->adrEnd)
		{
			cty.printf("%s(%s): Found in allocated memory block 1 %08X-%08X (%p)\n",
				device.tagName(), name, entry->adrStart, entry->adrEnd,
				entry->memory + address_to_byte(adrStart - entry->adrStart));

			result = (uint8_t *)entry->memory + address_to_byte(adrStart - entry->adrStart);
		}

	if (result != nullptr)
		return result;

	for (auto &block : manager.blocks())
		if (block->contains(*this, adrStart, adrEnd))
		{
			cty.printf("%s(%s): Found in allocated memory block 2 %08X-%08X (%p)\n",
				device.tagName(), name, block->start(), block->end(),
				block->base() + address_to_byte(adrStart - block->start()));

			return block->base() + address_to_byte(adrStart - block->start());
		}

	return result;
}


mapAddressEntry *mapAddressSpace::assignBlockIntersecting(const cty_t &cty, offs_t adrStart, offs_t adrEnd, uint8_t *base)
{
	return nullptr;
}

mapMemoryBank &mapAddressSpace::allocateBank(tag_t *tag, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type)
{
	offs_t adrMask = ~adrMirror;
	adjustAddresses(adrStart, adrEnd, adrMask, adrMirror);

	mapMemoryBank *bank = (tag != nullptr) ? manager.find(tag) : manager.find(*this, adrStart, adrEnd);

	if (bank == nullptr)
		bank = manager.allocateBank(*this, adrStart, adrEnd, tag);

	assert(bank != nullptr);
	bank->addReference(*this, type);
	return *bank;
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

mapMemoryBank *mapMemoryManager::find(tag_t *tag) const
{
	auto bank = bankList.find(tag);
	if (bank != bankList.end())
		return bank->second;

	return nullptr;
}

mapMemoryBank *mapMemoryManager::find(mapAddressSpace &space, offs_t adrStart, offs_t adrEnd)
{
	for (auto &bank : bankList)
		if (bank.second->isAnonymous() &&
			bank.second->hasReference(space, rwType::RW) &&
			bank.second->matchExcatly(adrStart, adrEnd))
			return bank.second;

	return nullptr;
}

mapMemoryBank *mapMemoryManager::allocateBank(mapAddressSpace &space, offs_t adrStart, offs_t adrEnd, tag_t *tag)
{
	auto bank = new mapMemoryBank(space, bankList.size(), adrStart, adrEnd, tag);
//	std::string newTag;
	char newTag[1024];

	if (tag == nullptr) {
//		newTag = strFormat("anon_%p", bank);
//		tag = newTag.c_str();
		sprintf(newTag, "anon_%p", bank);
		tag = newTag;
	}

	bankList.emplace(tag, std::move(bank));
	return bankList.find(tag)->second;
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

// ********************************************************************

mapMemoryBank::mapMemoryBank(mapAddressSpace &space, int index, offs_t sAddr, offs_t eAddr, tag_t *tag)
: system(*space.getManager().sysMachine()),
  space(space),
  anonymous(tag == nullptr),
  start(sAddr),
  end(eAddr),
  current(0)
{
	if (tag == nullptr) {
//		tagName  = strFormat("%d", index);
//		bankName = strFormat("Internal bank #%d", index);
		strTagName = "";
		strBankName = "";
	} else {
		strTagName = tag;
		strBankName = tag;
//		bankName = strFormat("Bank '%s'", tag);
	}

	// save manager
}

mapMemoryBank::~mapMemoryBank()
{
}

void mapMemoryBank::setBase(void *base)
{
	if (base == nullptr)
		return;

	if (entries.empty()) {
		entries.resize(1);
		current = 0;
	}

	entries[current] = reinterpret_cast<uint8_t *>(base);

	// Notify devices about that
	for (auto cb : notifiers)
		cb(base);
	notifiers.clear();
}

void mapMemoryBank::addReference(mapAddressSpace &space, rwType type)
{
	if (hasReference(space, type))
		return;
	refList.push_back(new bankReference(space, type));
}

bool mapMemoryBank::hasReference(const mapAddressSpace &space, rwType type) const
{
	for (auto &ref : refList)
		if (ref->matches(space, type))
			return true;
	return false;
}

void mapMemoryBank::addNotifier(std::function<void (void *)> cb)
{
	notifiers.emplace_back(cb);
}

void mapMemoryBank::setEntry(int entry)
{
	if (anonymous == true)
		return;
	if (entry < 0 || entry >= entries.size())
		return;
	if (entries[entry] == nullptr)
		return;

	current = entry;
}

void mapMemoryBank::configureEntry(int entry, void *base)
{
	if (entry < 0)
		return;
	if (entry >= entries.size())
		entries.resize(entry+1);

	entries[entry] = reinterpret_cast<uint8_t *>(base);
}

void mapMemoryBank::configureEntries(int sEntry, int nEntries, void *base, offs_t stride)
{
	if (sEntry + nEntries >= entries.size())
		entries.resize(sEntry + nEntries + 1);

	for (int entry = 0; entry < nEntries; entry++)
		entries[sEntry + entry] = reinterpret_cast<uint8_t *>(base) + (entry * stride);
}
