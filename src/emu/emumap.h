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

class mapManager {
public:
	mapManager(device_t *sys);
	~mapManager();

private:
};

class mapAddressSpaceConfig
{
public:
	mapAddressSpaceConfig();

private:
	const char *name;
	uint8_t		dataWidth;
	uint8_t		addrWidth;
	uint8_t		addrShift;
	uint8_t		pageShift;

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

