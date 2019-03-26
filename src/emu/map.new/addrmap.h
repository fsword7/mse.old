/*
 * addrmap.h - Address mapping entries
 *
 *  Created on: Mar 10, 2019
 *      Author: fswor
 */

#pragma once

class mapAddress;

enum mapAddressType {
	mapNone = 0,
	mapRAM,			// RAM type
	mapROM,			// ROM type
	mapPort,		// I/O port type
	mapNop,			// Non-existent type
	mapUnmapped,	// Unmapped type
	mapBank,		// Memory bank type
	mapDelegate,	// Device delegate (callback)
	mapSubmap		// Device submap type
};

struct mapHandler
{
	mapAddressType type = mapNone;
	uint8_t bits = 0;
	tag_t *tag = nullptr;
	tag_t *name = nullptr;
};

class mapAddressEntry
{
public:
	mapAddressEntry(device_t &device, mapAddress &map, offs_t start, offs_t end);

	// MAP/ROM space mapping
	mapAddressEntry &ram()		{ read.type = mapRAM, write.type = mapRAM; return *this; }
	mapAddressEntry &rom()		{ read.type = mapRAM, write.type = mapNop; return *this; }
	mapAddressEntry &ronly()	{ read.type = mapRAM; return *this; }
	mapAddressEntry &wonly()	{ write.type = mapRAM; return *this; }

	// NOP space mapping
	mapAddressEntry &noprw()	{ read.type = mapNop, write.type = mapNop; return *this; }
	mapAddressEntry &nopr()		{ read.type = mapNop; return *this; }
	mapAddressEntry &nopw()		{ write.type = mapNop; return *this; }

	mapAddressEntry &mask(offs_t bits)		{ addrMask = bits; return *this; }
	mapAddressEntry &mirror(offs_t bits)	{ addrMirror = bits; return *this; }
	mapAddressEntry &select(offs_t bits)	{ addrSelect = bits; return *this; }

	mapAddressEntry &region(tag_t *name, offs_t offset = 0)
		{ tagRegion = name; offRegion = offset; return *this; }
	mapAddressEntry &share(tag_t *name)		{ tagShare = name; return *this; }


	mapAddress	&map;			// Reference to map manager
	device_t	&device;		// Reference to device

	offs_t		addrStart;		// Start address
	offs_t		addrEnd;		// End address
	offs_t		addrMask;		// Mask address bits
	offs_t		addrMirror;		// Mirror address bits
	offs_t		addrSelect;		// Select address bits

	mapHandler	read, write;	// Read/write entry handler
	offs_t		offRegion;		// Region offset from base
	tag_t		*tagRegion;		// Region tag name
	tag_t		*tagShare;		// Share tag name

	void		*memory;		// Assigned memory space

};

class mapAddress
{
	mapAddress(device_t &device, int space);
	mapAddress(device_t &device, mapAddressEntry &entry);
	~mapAddress();

	mapAddressEntry operator ()(offs_t start, offs_t end);

	offs_t	gmask;			// Global addressing mask
	int		unmapValue;		// Unmapped value

private:
	device_t	&device;
	int			addrSpace;	// Address space type

	std::vector <mapAddressEntry> list;
};
