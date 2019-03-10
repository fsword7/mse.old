/*
 * dispatch.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map2/handler.h"

template <int highBits, int dataWidth, int addrShift, int Endian>
class mapMemoryHandlerReadDispatch : public mapMemoryHandlerRead
{
protected:
	static constexpr uint32_t lowBits  = determineLowBits(highBits, dataWidth, addrShift);
	static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
	static constexpr uint32_t count    = 1u << bitCount;


public:
	mapMemoryHandlerReadDispatch(mapAddressSpace &space,
		mapMemoryHandlerRead<dataWidth, addrShift, Endian> *handler = nullptr)
	: mapMemoryHandlerRead(space, mapMemoryHandler::heDispatch)
	{
		if (handler == nullptr);
		for (int ent = 0; ent < count; ent++) {
			dispatch[ent] = handler;
//			ranges[ent] = &init;
		}
	}
	~mapMemoryHandlerReadDispatch() { }

protected:
	mapMemoryHandlerRead<dataWidth, addrShift, Endian> *dispatch[count];

};

template <int highBits, int dataWidth, int addrShift, int Endian>
class mapMemoryHandlerWriteDispatch : public mapMemoryHandlerWrite
{
protected:
	static constexpr uint32_t lowBits  = determineLowBits(highBits, dataWidth, addrShift);
	static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
	static constexpr uint32_t count    = 1u << bitCount;

public:
	mapMemoryHandlerWriteDispatch(mapAddressSpace &space,
		mapMemoryHandlerWrite<dataWidth, addrShift, Endian> *handler = nullptr)
	: mapMemoryHandlerWrite(space, mapMemoryHandler::heDispatch)
	{
		if (handler == nullptr);
		for (int ent = 0; ent < count; ent++) {
			dispatch[ent] = handler;
//			ranges[ent] = &init;
		}
	}
	~mapMemoryHandlerWriteDispatch() { }

protected:
	mapMemoryHandlerRead<dataWidth, addrShift, Endian> *dispatch[count];

};
