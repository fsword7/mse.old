/*
 * emumap.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <type_traits>

class mapMemoryManager;
class mapAddress;
class mapAddressEntry;
class mapAddressSpace;
class mapMemoryBank;
class machine;

// Address space type
#define AS_PROGRAM		0
#define AS_DATA			1
#define AS_IO			2

enum class rwType {
	READ  = 1,
	WRITE = 2,
	RW    = 3
};

//using offs_t = std::size_t;
using offs_t = uint32_t;

using mapConstructor = named_delegate<void (mapAddress &)>;

using read8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using read16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using read32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using read64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

using write8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using write16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using write32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using write64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

#include "emu/map/he.h"
#include "emu/map/heun.h"


class mapAddressConfig
{
	friend class mapAddress;

public:
	mapAddressConfig();
	mapAddressConfig(tag_t *name, endian_t endian,
			uint8_t dWidth, uint8_t aWidth,
			int8_t aShift = 0,
			mapConstructor intmap = mapConstructor());

	tag_t *getName() const { return name; }

	endian_t endian() const		{ return endianness; }

	uint8_t data_width() const   { return dataWidth; }
	uint8_t address_width() const { return addrWidth; }
	int8_t  address_shift() const { return addrShift; }
	int8_t  page_shift() const   { return pageShift; }

	inline offs_t address_to_byte(offs_t address) const {
		return (addrShift < 0) ? (address << -addrShift) : (address >> addrShift);
	}

	inline offs_t byte_to_address(offs_t address) const {
		return (addrShift > 0) ? (address << addrShift) : (address >> -addrShift);
	}

	inline offs_t byte_to_address_end(offs_t address) const {
		return (addrShift < 0)  ? ((address << -addrShift) | ((1 << -addrShift) - 1))
								: (address >> addrShift);
	}

	inline offs_t address_to_byte_end(offs_t address) const {
		return (addrShift > 0)  ? ((address << addrShift) | ((1 << addrShift) - 1))
								: (address >> -addrShift);
	}

private:
	tag_t		*name;
	endian_t	endianness;
	uint8_t		dataWidth;
	uint8_t		addrWidth;
	int8_t		addrShift;
	int8_t		pageShift;

	mapConstructor internalMap;
};

class mapAddressSpace
{
protected:
	mapAddressSpace(mapMemoryManager &manager, di_memory &memeory, int space);

	inline void adjustAddresses(offs_t &start, offs_t &end, offs_t &mask, offs_t &mirror);

public:
	virtual ~mapAddressSpace();

	mapMemoryManager &getManager() const { return manager; }

	int data_width() const { return config.data_width(); }
	int addr_width() const { return config.address_width(); }
	int addr_shift() const { return config.address_shift(); }
	endian_t endian() const { return config.endian(); }
	uint64_t unmap() const { return unmapValue; }

	// Setup initialization routines
	void prepare(const cty_t &cty);
	void populateEntry(const cty_t &cty, const mapAddressEntry &entry, rwType type);
	void populate(const cty_t &cty, mapAddress *map = nullptr);
	void allocate(const cty_t &cty);
	void locate(const cty_t &cty);

	virtual void setup_unmap_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type, bool quiet) = 0;
	virtual void setup_ram_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type, void *base) = 0;
	virtual void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, std::string rtag, std::string wtag) = 0;
	virtual void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, mapMemoryBank *rabank, mapMemoryBank *wbank) = 0;

	// Address/byte conversion function calls
	offs_t address_to_byte(offs_t address) const { return config.address_to_byte(address); }
	offs_t address_to_byte_end(offs_t address) const { return config.address_to_byte_end(address); }
	offs_t byte_to_address(offs_t address) const { return config.byte_to_address(address); }
	offs_t byte_to_address_end(offs_t address) const { return config.byte_to_address_end(address); }

	template<int dWidth, int aShift, int Endian>
	mapReadHandlerUnmapped<dWidth, aShift, Endian> *getReadUnmap() const
	{ return nullptr; /* static_cast<mapReadHandlerUnmapped<dWidth, aShift, Endian>> readUnmap; */ }

	template<int dWidth, int aShift, int Endian>
	mapWriteHandlerUnmapped<dWidth, aShift, Endian> *getWriteUnmap() const
	{ return nullptr; /* static_cast<mapWriteHandlerUnmapped<dWidth, aShift, Endian>> writeUnmap; */ }

	// read accessors
//	virtual uint8_t read8(offs_t address) = 0;
//	virtual uint16_t read16(offs_t address) = 0;
//	virtual uint16_t read16(offs_t address, uint16_t mask) = 0;
//	virtual uint16_t read16u(offs_t address) = 0;
//	virtual uint16_t read16u(offs_t address, uint16_t mask) = 0;
//	virtual uint32_t read32(offs_t address) = 0;
//	virtual uint32_t read32(offs_t address, uint32_t mask) = 0;
//	virtual uint32_t read32u(offs_t address) = 0;
//	virtual uint32_t read32u(offs_t address, uint32_t mask) = 0;
//	virtual uint64_t read64(offs_t address) = 0;
//	virtual uint64_t read64(offs_t address, uint64_t mask) = 0;
//	virtual uint64_t read64u(offs_t address) = 0;
//	virtual uint64_t read64u(offs_t address, uint64_t mask) = 0;

	// write accessors
//	virtual void write8(offs_t address, uint8_t data) = 0;
//	virtual void write16(offs_t address, uint16_t data) = 0;
//	virtual void write16(offs_t address, uint16_t data, uint16_t mask) = 0;
//	virtual void write16u(offs_t address, uint16_t data) = 0;
//	virtual void write16u(offs_t address, uint16_t data, uint16_t mask) = 0;
//	virtual void write32(offs_t address, uint32_t data) = 0;
//	virtual void write32(offs_t address, uint32_t data, uint32_t mask) = 0;
//	virtual void write32u(offs_t address, uint32_t data) = 0;
//	virtual void write32u(offs_t address, uint32_t data, uint32_t mask) = 0;
//	virtual void write64(offs_t address, uint64_t data) = 0;
//	virtual void write64(offs_t address, uint64_t data, uint64_t mask) = 0;
//	virtual void write64u(offs_t address, uint64_t data) = 0;
//	virtual void write64u(offs_t address, uint64_t data, uint64_t mask) = 0;

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
	const mapAddressConfig &config;	// address space configuration
	const char			*name;		// Name of the address space
	int					space;		// Address space index
	device_t			&device;	// Reference to the owning device
	mapMemoryManager	&manager;	// Reference to the owning map manager
	mapAddress			*map;		// Original address map database

	offs_t		addrMask;	// Address mask

	uint64_t	unmapValue;	// Unmapped value

	mapHandlerEntry *readUnmap;
	mapHandlerEntry *writeUnmap;
	mapHandlerEntry *readNop;
	mapHandlerEntry *writeNop;

};


class mapMemoryBlock
{
public:
	mapMemoryBlock(mapAddressSpace &space, offs_t start, offs_t end, void *memory = nullptr);
	~mapMemoryBlock() {}

	// getters
	machine &sysMachine() const { return system; }
	offs_t start() const { return addrStart; }
	offs_t end() const { return addrEnd; }
	uint8_t *base() const { return baseData; }

	// Check address range within memory block
	bool contains(mapAddressSpace &space, offs_t sAddr, offs_t eAddr) const
	{
		return (&space == &addrSpace && addrStart <= sAddr && addrEnd >= eAddr);
	}

private:
	machine			&system;
	mapAddressSpace &addrSpace;
	offs_t			addrStart;
	offs_t			addrEnd;
	uint8_t			*baseData;

	std::vector<uint8_t> allocated;
};

class mapMemoryBank
{
public:
	mapMemoryBank(mapAddressSpace &space, offs_t start, offs_t end, tag_t *tag = nullptr)
	: space(space), start(start), end(end), tag(tag)
	{ }

	const std::string &tagName() const { return tag; }

//	machine &sysMachine() { return system; }
	int  entry() const { return current; }
	bool anonymous() const { return flagAnonymous; }
	offs_t addrStart() const { return start; }
//	tag_t *name() const { return bankName.c_str(); }
//	tag_t *tag() const { return tagName.c_str(); }

	void *base() const
	{
		return !entries.empty() ? entries[current] : nullptr;
	}

	// Configure/set entries
//	void configureEntry(int entry, void *base);
//	void configureEntries(int sEntry, int nEntries, void *base, offs_t stride);
//	void setEntry(int entry);

private:
//	machine			&system;
	mapAddressSpace &space;
	offs_t			start, end;
	int				current;
	std::vector<uint8_t *> entries;
	bool			flagAnonymous;
	std::string		bank;
	std::string		tag;
};

class mapMemoryShare
{
public:
	mapMemoryShare(uint8_t width, size_t size, endian_t endian, void *data = nullptr)
	: baseData(data), dataSize(size),
	  endianType(endian), bitWidth(width),
	  byteWidth(width / 8)
	{ }

	// getters
	void *base() const { return baseData; }
	size_t size() const { return dataSize; }
	endian_t endian() const { return endianType; }
	uint8_t bit_width() const { return bitWidth; }
	uint8_t byte_width() const { return byteWidth; }

	// setters
	void setBase(void *base) { baseData = base; }

private:
	void		*baseData;
	size_t		dataSize;
	endian_t	endianType;
	uint8_t		bitWidth;
	uint8_t		byteWidth;
};

class mapMemoryRegion
{
public:
	mapMemoryRegion(machine *sys, tag_t *tag, uint32_t length, uint8_t width, endian_t endian)
	: sysMachine(sys), tagName(tag), data(length),
	  endianType(endian), bitWidth(width * 8), byteWidth(width)
	{
		assert(width == 1 || width == 2 || width == 4 | width == 8);
	}

	uint8_t *base() { return (data.size() > 0) ? &data[0] : nullptr; }
	uint8_t *end()	{ return base() + data.size(); }

	machine *system() const		{ return sysMachine; }
	tag_t *name() const			{ return tagName.c_str(); }
	endian_t endian() const		{ return endianType; }
	uint32_t size()	const		{ return data.size(); }
	uint8_t bit_width() const	{ return bitWidth; }
	uint8_t byte_width() const	{ return byteWidth; }

	// Data access
	uint8_t	&access8(offs_t offset = 0)		{ return data[offset]; }
	uint16_t &access16(offs_t offset = 0)	{ return reinterpret_cast<uint16_t *>(base())[offset]; }
	uint32_t &access32(offs_t offset = 0)	{ return reinterpret_cast<uint32_t *>(base())[offset]; }
	uint64_t &access64(offs_t offset = 0)	{ return reinterpret_cast<uint64_t *>(base())[offset]; }

private:
	machine					*sysMachine;
	std::string				tagName;
	std::vector<uint8_t>	data;
	endian_t				endianType;
	uint8_t					bitWidth;
	uint8_t					byteWidth;
};

class mapMemoryManager {
public:
	mapMemoryManager(machine *sys);
	~mapMemoryManager();

	machine *sysMachine() { return system; }

	const std::map<std::string, mapMemoryRegion *> &regions() const { return regionList; }
	const std::map<std::string, mapMemoryBank *> &banks() const { return bankList; }
	const std::map<std::string, mapMemoryShare *> &shares() const { return shareList; }

	void allocate(const cty_t &cty, di_memory &memory);
	void init(const cty_t &cty);

	// Memory regions
	mapMemoryRegion *allocateRegion(tag_t *name, uint32_t length, uint8_t width, endian_t endian);
	void freeRegion(tag_t *name);

private:
	machine *system;


	std::vector<mapMemoryBlock *> blockList;
	std::map<std::string, mapMemoryRegion *> regionList;
	std::map<std::string, mapMemoryBank *> bankList;
	std::map<std::string, mapMemoryShare *> shareList;
};
