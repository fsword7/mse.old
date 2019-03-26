/*
 * handler.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Tim Stark
 */

#pragma once

using offs_t = uint32_t;

// Data Width - types
template <int datawidth> struct mapHandlerSize { };
template <> struct mapHandlerSize<0> { using uintx_t = uint8_t; }
template <> struct mapHandlerSize<1> { using uintx_t = uint16_t; }
template <> struct mapHandlerSize<2> { using uintx_t = uint32_t; }
template <> struct mapHandlerSize<3> { using uintx_t = uint64_t; }

class mapMemoryHandler
{
public:
	enum {
		heDispatch = 0x0001
	};

	mapMemoryHandler(mapAddressSpace &space, uint32_t flags)
	: space(space), flags(flags), refCount(1) { }
	virtual ~mapMemoryHandler() { }

	// Reference count control

	void ref(int count = 1)
	{
		refCount += count;
	}

	void unref(int count = 1) {
		refCount -= count;
		if (refCount == 0)
			delete this;
	}

private:
	mapAddressSpace &space;		// Address space
	uint32_t		flags;		// Memory handler flags
	mutable int 	refCount;	// Reference count
};

template <int dataWidth, int addrShift, int Endian>
class mapMemoryHandlerRead : public mapMemoryHandler
{
public:
	using uintx_t = typename mapHandlerSize<dataWidth>::uintx_t;

	mapMemoryHandlerRead(mapAddressSpace &space, uint32_t flags)
	: mapMemoryHandler(space, flags) { }
	virtual ~mapMemoryHandlerRead() { }

	virtual uintx_t read(offs_t offset, uintx_t mask) = 0;

	void populate(const cty_t &cty, offs_t &start, offs_t &end, offs_t mirror,
		mapMemoryHandlerRead<dataWidth, addrShift, Endian> *handler)
	{
	}
};

template <int dataWidth, int addrShift, int Endian>
class mapMemoryHandlerWrite : public mapMemoryHandler
{
public:
	using uintx_t = typename mapHandlerSize<dataWidth>::uintx_t;

	mapMemoryHandlerWrite(mapAddressSpace &space, uint32_t flags)
	: mapMemoryHandler(space, flags) { }
	virtual ~mapMemoryHandlerWrite() { }

	virtual uintx_t write(offs_t offset, uintx_t data, uintx_t mask) = 0;

	void populate(const cty_t &cty, offs_t &start, offs_t &end, offs_t mirror,
		mapMemoryHandlerWrite<dataWidth, addrShift, Endian> *handler)
	{
	}
};
