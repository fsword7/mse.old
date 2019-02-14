/*
 * emumap.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <type_traits>

class mapAddress;
class mapAddressSpace;
class mapMemoryBank;
class machine;

// Address space type
#define AS_PROGRAM		0
#define AS_DATA			1
#define AS_IO			2

using offs_t = std::size_t;

using mapConstructor = named_delegate<void (mapAddress &)>;

using read8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using read16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using read32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using read64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

using write8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using write16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using write32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using write64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

#include "emu/emumap_he.h"
#include "emu/emumap_hed.h"

class mapManager {
public:
	mapManager(machine *sys);
	~mapManager();

	void allocate(di_memory &memory);
	void init();

private:
	machine *system;
};

class mapAddressConfig
{
	friend class mapAddress;

public:
	mapAddressConfig();
	mapAddressConfig(tag_t *name, endian_t endian,
			uint8_t dWidth, uint8_t aWidth,
			int8_t aShift = 0,
			mapConstructor intmap = mapConstructor(),
			mapConstructor defmap = mapConstructor());

	tag_t *getName() { return name; }

private:
	tag_t		*name;
	endian_t	endianness;
	uint8_t		dataWidth;
	uint8_t		addrWidth;
	int8_t		addrShift;
	int8_t		pageShift;

	mapConstructor internalMap;
	mapConstructor defaultMap;
};

class mapAddressSpace
{
protected:
	mapAddressSpace(mapManager &manager, di_memory &memeory, int space);

public:
	virtual ~mapAddressSpace();

	// Setup initialization routines
	void prepare();
	void populate(mapAddress *map = nullptr);
	void allocate();
	void locate();

	// read accessors
	virtual uint8_t read8(offs_t address) = 0;
	virtual uint16_t read16(offs_t address) = 0;
	virtual uint16_t read16(offs_t address, uint16_t mask) = 0;
	virtual uint16_t read16u(offs_t address) = 0;
	virtual uint16_t read16u(offs_t address, uint16_t mask) = 0;
	virtual uint32_t read32(offs_t address) = 0;
	virtual uint32_t read32(offs_t address, uint32_t mask) = 0;
	virtual uint32_t read32u(offs_t address) = 0;
	virtual uint32_t read32u(offs_t address, uint32_t mask) = 0;
	virtual uint64_t read64(offs_t address) = 0;
	virtual uint64_t read64(offs_t address, uint64_t mask) = 0;
	virtual uint64_t read64u(offs_t address) = 0;
	virtual uint64_t read64u(offs_t address, uint64_t mask) = 0;

	// write accessors
	virtual void write8(offs_t address, uint8_t data) = 0;
	virtual void write16(offs_t address, uint16_t data) = 0;
	virtual void write16(offs_t address, uint16_t data, uint16_t mask) = 0;
	virtual void write16u(offs_t address, uint16_t data) = 0;
	virtual void write16u(offs_t address, uint16_t data, uint16_t mask) = 0;
	virtual void write32(offs_t address, uint32_t data) = 0;
	virtual void write32(offs_t address, uint32_t data, uint32_t mask) = 0;
	virtual void write32u(offs_t address, uint32_t data) = 0;
	virtual void write32u(offs_t address, uint32_t data, uint32_t mask) = 0;
	virtual void write64(offs_t address, uint64_t data) = 0;
	virtual void write64(offs_t address, uint64_t data, uint64_t mask) = 0;
	virtual void write64u(offs_t address, uint64_t data) = 0;
	virtual void write64u(offs_t address, uint64_t data, uint64_t mask) = 0;

//	// set ports, banks and RAM access
//	void setRAM(offs_t start, offs_t end, void *base = nullptr);
//	void setROM(offs_t start, offs_t end, void *base = nullptr);
//	void setWriteOnly(offs_t start, offs_t end, void *base = nullptr);
//
//	void setPortR(offs_t start, offs_t end, tag_t *tag);
//	void setPortW(offs_t start, offs_t end, tag_t *tag);
//	void setPortRW(offs_t start, offs_t end, tag_t *tag);
//
//	void setBankR(offs_t start, offs_t end, tag_t *tag);
//	void setBankW(offs_t start, offs_t end, tag_t *tag);
//	void setBankRW(offs_t start, offs_t end, tag_t *tag);
//
//	void setBankR(offs_t start, offs_t end, mapMemoryBank *bank);
//	void setBankW(offs_t start, offs_t end, mapMemoryBank *bank);
//	void setBankRW(offs_t start, offs_t end, mapMemoryBank *bank);
//
//	// set ports, banks and RAM access (with mirror/mask)
//	void setRAM(offs_t start, offs_t end, offs_t mirror, void *base = nullptr);
//	void setROM(offs_t start, offs_t end, offs_t mirror, void *base = nullptr);
//	void setWriteOnly(offs_t start, offs_t end, offs_t mirror, void *base = nullptr);
//
//	void setPortR(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//	void setPortW(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//	void setPortRW(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//
//	void setBankR(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//	void setBankW(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//	void setBankRW(offs_t start, offs_t end, offs_t mirror, tag_t *tag);
//
//	void setBankR(offs_t start, offs_t end, offs_t mirror, mapMemoryBank *bank);
//	void setBankW(offs_t start, offs_t end, offs_t mirror, mapMemoryBank *bank);
//	void setBankRW(offs_t start, offs_t end, offs_t mirror, mapMemoryBank *bank);

protected:
	const char	*name;		// Name of the address space
	int			space;		// Address space index
	device_t	&device;	// Reference to the owning device
	mapManager	&manager;	// Reference to the owning map manager
	mapAddress	*map;		// Original address map database

	offs_t		addrMask;	// Address mask
};


class mapMemoryBlock
{
public:
	mapMemoryBlock(mapAddressSpace &space, offs_t start, offs_t end, uint8_t *memory = nullptr)
	: space(space), start(start), end(end), data(memory)
	{ }

private:
	mapAddressSpace &space;
	offs_t			start, end;
	uint8_t			*data;
};

class mapMemoryBank
{
public:
	mapMemoryBank(mapAddressSpace &space, offs_t start, offs_t end, tag_t *tag = nullptr)
	: space(space), start(start), end(end), tag(tag)
	{ }

private:
	mapAddressSpace &space;
	offs_t			start, end;
	std::string		tag;
};

class mapMemoryShare
{
};

class mapMemoryRegion
{
};
