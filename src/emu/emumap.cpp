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
//		install_ram_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, nullptr);
		break;

	case mapNop:
//		install_unmap_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, true);
		break;

	case mapUnmapped:
//		install_unmap_generic(entry.adrStart, entry.adrEnd, entry.adrMirror, type, false);
		break;

	case mapPort:
//		install_rw_port(entry.adrStart, entry.adrEnd, entry.adrMirror,
//				(type == rwType::READ) ? "" : ""),
//				(type == rwType::WRITE) ? "" : ""));
		break;

	case mapBank:
//		install_bank_port(entry.adrStart, entry.adrEnd, entry.adrMirror,
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

// ***********************************************************

template <int dWidth, int aShift, int Endian>
class mapAddressSpaceAccess : public mapAddressSpace
{
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using thisType = mapAddressSpaceAccess<dWidth, aShift, Endian>;
	using nativeType = uintx_t;

	static constexpr uint32_t nativeBytes = 1 << dWidth;
	static constexpr uint32_t nativeBits = 8 * nativeBytes;
	static constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes << labs(aShift) : nativeBytes >> labs(aShift);
	static constexpr uint32_t nativeMask = nativeStep - 1;

	static constexpr offs_t offsetToByte(offs_t offset)
	{
		return aShift < 0 ? offset << labs(aShift) : offset >> labs(aShift);
	}

public:
	mapAddressSpaceAccess(mapMemoryManager &manager, di_memory &memory, int space, int addrWidth)
	: mapAddressSpace(manager, memory, space),
	  rootRead(nullptr), rootWrite(nullptr)
	{

		readUnmap = new mapReadHandlerUnmapped<dWidth, aShift, Endian>(this);
		writeUnmap = new mapWriteHandlerUnmapped<dWidth, aShift, Endian>(this);
		readNop = new mapReadHandlerNop<dWidth, aShift, Endian>(this);
		writeNop = new mapWriteHandlerNop<dWidth, aShift, Endian>(this);

		mapHandlerEntry::range r = { 0, 0xFFFFFFFF >> (32 - addrWidth) };

		switch (addrWidth) {

		case 1:
			rootRead  = new mapReadHandlerDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 2:
			rootRead  = new mapReadHandlerDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 3:
			rootRead  = new mapReadHandlerDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 4:
			rootRead  = new mapReadHandlerDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 5:
			rootRead  = new mapReadHandlerDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 6:
			rootRead  = new mapReadHandlerDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 7:
			rootRead  = new mapReadHandlerDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 8:
			rootRead  = new mapReadHandlerDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 9:
			rootRead  = new mapReadHandlerDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 10:
			rootRead  = new mapReadHandlerDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 11:
			rootRead  = new mapReadHandlerDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 12:
			rootRead  = new mapReadHandlerDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 13:
			rootRead  = new mapReadHandlerDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 14:
			rootRead  = new mapReadHandlerDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 15:
			rootRead  = new mapReadHandlerDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 16:
			rootRead  = new mapReadHandlerDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 17:
			rootRead  = new mapReadHandlerDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 18:
			rootRead  = new mapReadHandlerDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 19:
			rootRead  = new mapReadHandlerDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 20:
			rootRead  = new mapReadHandlerDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 21:
			rootRead  = new mapReadHandlerDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 22:
			rootRead  = new mapReadHandlerDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 23:
			rootRead  = new mapReadHandlerDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 24:
			rootRead  = new mapReadHandlerDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 25:
			rootRead  = new mapReadHandlerDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 26:
			rootRead  = new mapReadHandlerDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 27:
			rootRead  = new mapReadHandlerDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 28:
			rootRead  = new mapReadHandlerDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 29:
			rootRead  = new mapReadHandlerDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 30:
			rootRead  = new mapReadHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 31:
			rootRead  = new mapReadHandlerDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 32:
			rootRead  = new mapReadHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		default:
			std::cerr << "Unhandled address bus width: " << addrWidth << std::endl;
		}
	}

	nativeType readNative(offs_t offset, nativeType mask)
	{
		return rootRead->read(offset, mask);
	}

	nativeType readNative(offs_t offset)
	{
		return rootRead->read(offset, uintx_t(0xFFFFFFFFFFFFFFFFu));
	}

	nativeType writeNative(offs_t offset, nativeType data, nativeType mask)
	{
		return rootWrite->write(offset, data, mask);
	}

	nativeType writeNative(offs_t offset, nativeType data)
	{
		return rootWrite->write(offset, data, uintx_t(0xFFFFFFFFFFFFFFFFu));
	}

	// read accessors
//	uint8_t read8(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16(offs_t address, uint16_t mask) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16u(offs_t address, uint16_t mask) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32(offs_t address, uint32_t mask) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32u(offs_t address, uint32_t mask) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64(offs_t address, uint64_t mask) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64u(offs_t address, uint64_t mask) override
//		{ address &= addrMask; return 0; }

	// write accessors
//	void write8(offs_t address, uint8_t data) override
//		{ address &= addrMask;  }
//	void write16(offs_t address, uint16_t data) override
//		{ address &= addrMask;  }
//	void write16(offs_t address, uint16_t data, uint16_t mask) override
//		{ address &= addrMask;  }
//	void write16u(offs_t address, uint16_t data) override
//		{ address &= addrMask;  }
//	void write16u(offs_t address, uint16_t data, uint16_t mask) override
//		{ address &= addrMask;  }
//	void write32(offs_t address, uint32_t data) override
//		{ address &= addrMask;  }
//	void write32(offs_t address, uint32_t data, uint32_t mask) override
//		{ address &= addrMask;  }
//	void write32u(offs_t address, uint32_t data) override
//		{ address &= addrMask;  }
//	void write32u(offs_t address, uint32_t data, uint32_t mask) override
//		{ address &= addrMask;  }
//	void write64(offs_t address, uint64_t data) override
//		{ address &= addrMask;  }
//	void write64(offs_t address, uint64_t data, uint64_t mask) override
//		{ address &= addrMask;  }
//	void write64u(offs_t address, uint64_t data) override
//		{ address &= addrMask;  }
//	void write64u(offs_t address, uint64_t data, uint64_t mask) override
//		{ address &= addrMask;  }


//	// static access to functions
//	static uint8_t read8s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint16_t read16s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint16_t read16ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//	static uint32_t read32s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint32_t read32ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//	static uint64_t read64s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint64_t read64ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//
//	static void write8s(type &space, offs_t address, uint8_t data)
//	{ address &= space.addrMask;  }
//	static void write16s(type &space, offs_t address, uint16_t data)
//	{ address &= space.addrMask;  }
//	static void write16ms(type &space, offs_t address, uint16_t data, offs_t mask)
//	{ address &= space.addrMask;  }
//	static void write32s(type &space, offs_t address, uint32_t data)
//	{ address &= space.addrMask;  }
//	static void write32ms(type &space, offs_t address, uint32_t data, offs_t mask)
//	{ address &= space.addrMask;  }
//	static void write64s(type &space, offs_t address, uint64_t data)
//	{ address &= space.addrMask;  }
//	static void write64ms(type &space, offs_t address, uint64_t data, offs_t mask)
//	{ address &= space.addrMask;  }


	mapReadHandlerEntry<dWidth, aShift, Endian>  *rootRead;
	mapWriteHandlerEntry<dWidth, aShift, Endian> *rootWrite;
};

// ******************************************************************

mapMemoryManager::mapMemoryManager(machine *sys)
: system(sys)
{
}

mapMemoryManager::~mapMemoryManager()
{
}

void mapMemoryManager::allocate(di_memory &memory)
{
	std::cout << memory.getDevice()->deviceName() <<
		": Allocating address spaces (" << memory.mapConfigCount() << " spaces)" << std::endl;

	for (int space = 0; space < memory.mapConfigCount(); space++) {
		const mapAddressConfig *config = memory.getAddressSpaceConfig(space);
		if (config != nullptr) {
			switch( config->address_width() | (4 - config->address_shift())) {
			// 8-bit address width
			case 8|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<0, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<0, 0, endianLittle>>(*this, space);
				break;

			// 16-bit address width
			case 16|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<1, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<1, 0, endianLittle>>(*this, space);
				break;
			case 16|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<1, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<1, -1, endianLittle>>(*this, space);
				break;

			// 32-bit address width
			case 32|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, 0, endianLittle>>(*this, space);
				break;
			case 32|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, -1, endianLittle>>(*this, space);
				break;
			case 32|(4-2):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, -2, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, -2, endianLittle>>(*this, space);
				break;

			// 64-bit address width
			case 64|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, 0, endianLittle>>(*this, space);
				break;
			case 64|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -1, endianLittle>>(*this, space);
				break;
			case 64|(4-2):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -2, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -2, endianLittle>>(*this, space);
				break;
			case 64|(4-3):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -3, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -3, endianLittle>>(*this, space);
				break;

			default:
				throw mseFatalError("Address Space: Invalid address width %d shift %d",
					config->address_width(), config->address_shift());
			}
		}
	}
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
