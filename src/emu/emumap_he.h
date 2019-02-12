/*
 * emumap_he.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/emumap.h"

class mapHandlerEntry {
public:
	mapHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: space(space), flags(flags), refCount(1)
	{ }

	virtual ~mapHandlerEntry();

protected:
	mapAddressSpace		*space;
	uint32_t			flags;
	mutable uint32_t	refCount;
};

template <int DataWidth, int AddrShift, endian_t Endian>
class mapReadHandlerEntry : public mapHandlerEntry
{
public:
	mapReadHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	~mapReadHandlerEntry();

};

template <int DataWidth, int AddrShift, endian_t Endian>
class mapWriteHandlerEntry : public mapHandlerEntry
{
public:
	mapWriteHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	~mapWriteHandlerEntry();


};
