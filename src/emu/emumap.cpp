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
	dimem_iterator iter(*system->getSystemDevice());
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

mapAddressSpace::mapAddressSpace(mapManager &manager, di_memory &memory, int space)
: name(memory.getAddressSpaceConfig(space)->getName()),
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

// ***********************************************************

template <int dWidth, int aShift, endian_t Endian>
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
	mapAddressSpaceAccess(mapManager &manager, di_memory &memory, int space, int addrWidth)
	: mapAddressSpace(manager, memory, space),
	  rootRead(nullptr), rootWrite(nullptr)
	{

		mapHandlerEntry::range r(0, 0xFFFFFFFF >> (32 - addrWidth));

//		switch (addrWidth) {
//
//		case 30:
//			rootRead = new mapReadHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
//			rootWrite = new mapWriteHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
//			break;
//
//		case 32:
//			rootRead = new mapReadHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
//			rootWrite = new mapWriteHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
//			break;
//
//		default:
//			std::cerr << "Unhandled address bus width: " << addrWidth << std::endl;
//		}
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
	uint8_t read8(offs_t address) override
		{ address &= addrMask; return 0; }
	uint16_t read16(offs_t address) override
		{ address &= addrMask; return 0; }
	uint16_t read16(offs_t address, uint16_t mask) override
		{ address &= addrMask; return 0; }
	uint16_t read16u(offs_t address) override
		{ address &= addrMask; return 0; }
	uint16_t read16u(offs_t address, uint16_t mask) override
		{ address &= addrMask; return 0; }
	uint32_t read32(offs_t address) override
		{ address &= addrMask; return 0; }
	uint32_t read32(offs_t address, uint32_t mask) override
		{ address &= addrMask; return 0; }
	uint32_t read32u(offs_t address) override
		{ address &= addrMask; return 0; }
	uint32_t read32u(offs_t address, uint32_t mask) override
		{ address &= addrMask; return 0; }
	uint64_t read64(offs_t address) override
		{ address &= addrMask; return 0; }
	uint64_t read64(offs_t address, uint64_t mask) override
		{ address &= addrMask; return 0; }
	uint64_t read64u(offs_t address) override
		{ address &= addrMask; return 0; }
	uint64_t read64u(offs_t address, uint64_t mask) override
		{ address &= addrMask; return 0; }

	// write accessors
	void write8(offs_t address, uint8_t data) override
		{ address &= addrMask;  }
	void write16(offs_t address, uint16_t data) override
		{ address &= addrMask;  }
	void write16(offs_t address, uint16_t data, uint16_t mask) override
		{ address &= addrMask;  }
	void write16u(offs_t address, uint16_t data) override
		{ address &= addrMask;  }
	void write16u(offs_t address, uint16_t data, uint16_t mask) override
		{ address &= addrMask;  }
	void write32(offs_t address, uint32_t data) override
		{ address &= addrMask;  }
	void write32(offs_t address, uint32_t data, uint32_t mask) override
		{ address &= addrMask;  }
	void write32u(offs_t address, uint32_t data) override
		{ address &= addrMask;  }
	void write32u(offs_t address, uint32_t data, uint32_t mask) override
		{ address &= addrMask;  }
	void write64(offs_t address, uint64_t data) override
		{ address &= addrMask;  }
	void write64(offs_t address, uint64_t data, uint64_t mask) override
		{ address &= addrMask;  }
	void write64u(offs_t address, uint64_t data) override
		{ address &= addrMask;  }
	void write64u(offs_t address, uint64_t data, uint64_t mask) override
		{ address &= addrMask;  }


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
