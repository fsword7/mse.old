/*
 * addrmap.h - Address space mapping
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

class mapAddress;

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
	mapAddressEntry(device_t &device, mapAddress &map, offs_t start, offs_t end);

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

	mapAddressEntry &region(tag_t *tag, offs_t offset = 0)
		{ tagRegion = tag; rgnOffset = offset; return *this; }
	mapAddressEntry &share(tag_t *tag) { tagShare = tag; return *this; }

	mapAddressEntry &mask(offs_t mask);

	// Address entry information (public access)
	mapAddressEntry *mapNext;	// Point to the next entry
	mapAddress   	&map;		// Reference to address map database
	device_t		&device;	// Reference to base device

	// Address parameters (public address)
	offs_t		adrStart;	// Start address
	offs_t		adrEnd;		// End address
	offs_t		adrMask;	// Mask address bits
	offs_t		adrMirror;	// Mirror address bits
	offs_t		adrSelect;	// Select address bits

	mapHandler  read, write;	// Memory map handler for read and write access
	tag_t		*tagShare;		// Tag of shared map block
	tag_t		*tagRegion;		// Tag of region map block
	offs_t		rgnOffset;		// Offset from region base

	device_t		*submapDevice;
	mapConstructor	submapDelegate;

	// access handlers
	read8_delegate		read8;
	read16_delegate		read16;
	read32_delegate		read32;
	read64_delegate		read64;

	write8_delegate		write8;
	write16_delegate	write16;
	write32_delegate	write32;
	write64_delegate	write64;

};

class mapAddress {
public:
	mapAddress(device_t &device, int space);
	mapAddress(device_t &device, mapAddressEntry *entry);
	~mapAddress();

	mapAddressEntry &operator()(offs_t start, offs_t end);

	void setGlobalMask(offs_t mask) { gmask = mask; }

	void validate(int space) const;

	offs_t	gmask;	// Global bit mask

private:
	device_t &device;
	int       adrSpace;
	std::vector<mapAddressEntry> list;
};

