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

// Address space type
#define AS_PROGRAM		0
#define AS_DATA			1
#define AS_IO			2

using offs_t = std::size_t;

using mapConstructor = named_delegate<void (mapAddressSpace &)>;

using read8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using read16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using read32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using read64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

using write8_delegate = device_delegate<uint8_t(mapAddress &, offs_t, uint8_t)>;
using write16_delegate = device_delegate<uint16_t(mapAddress &, offs_t, uint16_t)>;
using write32_delegate = device_delegate<uint32_t(mapAddress &, offs_t, uint32_t)>;
using write64_delegate = device_delegate<uint64_t(mapAddress &, offs_t, uint64_t)>;

class mapManager {
public:
	mapManager(device_t *sys);
	~mapManager();

private:
};

class mapAddressConfig
{
public:
	mapAddressConfig();
	mapAddressConfig(tag_t *name, endian_t endian,
			uint8_t dWidth, uint8_t aWidth,
			int8_t aShift = 0,
			mapConstructor intmap = mapConstructor(),
			mapConstructor defmap = mapConstructor());

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
	const char	*name;		// Name of the address space
	device_t	&device;	// Reference to the owning device
	mapManager	&manager;	// Reference to the owning map manager
	mapAddress	*map;		// Original address map database
};

