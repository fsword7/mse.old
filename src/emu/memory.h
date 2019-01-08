/*
 * memory.h - Memory Manager facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

// Address space type
#define AS_PROGRAM		0
#define AS_DATA			1
#define AS_IO			2

class Device;
class mapAddress;
class mapAddressSpace;

using offs_t = std::size_t;

using mapConstructor = named_delegate<void (mapAddressSpace &)>;

using read8_delegate = device_delegate<uint8_t (mapAddress &, offs_t, uint8_t)>;

class mapManager {
public:
	mapManager(Device *sys);
	~mapManager();

private:
};

enum mapType {
	mapNone = 0,
	mapRAM,			// RAM type
	mapROM,			// ROM type
	mapPort,		// I/O port type
	mapNop,			// Non-existent type
	mapBank,		// Memory bank type
	mapDelegate,	// Device delegate (callback)
	mapSubmap		// Device submap type
};

struct mapHandler {
//	mapHandler() {
//		type = mapNone;
//		bits = 0;
//		tag  = nullptr;
//		name = nullptr;
//	}

	mapType		type  = mapNone;		// Mapping access type
	uint8_t		bits  = 0;			// Bit width (0 for default)
	const char	*tag  = nullptr;		// Tag name for named ports (I/O and banks)
	const char	*name = nullptr;	// Handler name
};

class mapAddressEntry {
public:
	mapAddressEntry(Device &dev, mapAddress &map, offs_t start, offs_t end);

	// Mapping parameter setting function calls

	// RAM/ROM mapping
	mapAddressEntry &ram()		{ read.type = mapRAM; write.type = mapRAM; return *this; }
	mapAddressEntry &rom()		{ read.type = mapROM; write.type = mapNop; return *this; }
	mapAddressEntry &ronly()	{ read.type = mapRAM; return *this; }
	mapAddressEntry &wonly()	{ write.type = mapRAM; return *this; }

	// Non-existent access mapping
	mapAddressEntry &noprw()	{ read.type = mapNop; write.type = mapNop; return *this; }
	mapAddressEntry &nopr()		{ read.type = mapNop; return *this; }
	mapAddressEntry &nopw()		{ write.type = mapNop; return *this; }

	mapAddressEntry &mirror(offs_t bits) { adrMirror = bits; return *this; }
	mapAddressEntry &select(offs_t bits) { adrSelect = bits; return *this; }

	mapAddressEntry &share(const char *tag) { tagShare = tag; return *this; }

	mapAddressEntry &mask(offs_t mask);

	// Address entry information (public access)
	mapAddressEntry *mapNext;	// Point to the next entry
	mapAddress   	&map;	// Reference to address map database
	Device			&dev;	// Reference to base device

	// Address parameters (public address)
	offs_t		adrStart;	// Start address
	offs_t		adrEnd;		// End address
	offs_t		adrMask;	// Mask address bits
	offs_t		adrMirror;	// Mirror address bits
	offs_t		adrSelect;	// Select address bits

	mapHandler  read, write;	// Memory map handler for read and write access
	const char	*tagShare;		// Tag of shared map block


};

class mapAddress {
public:
	mapAddress(Device &sys);
	~mapAddress();

	mapAddressEntry &operator()(offs_t start, offs_t end);

	void setGlobalMask(offs_t mask) { gmask = mask; }

	offs_t	gmask;	// Global bit mask

private:
	Device &sys;
	std::vector<mapAddressEntry> list;
};

class mapAddressSpaceConfig
{

};

class mapAddressSpace
{

protected:
	const char	*name;		// Name of the address space
	device_t	&device;	// Reference to the owning device
	mapManager	&manager;	// Reference to the owning map manager
	mapAddress	*map;		// Original address map database
};
