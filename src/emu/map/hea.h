/*
 * hea.h
 *
 *  Created on: Mar 4, 2019
 *      Author: fswor
 */

#pragma once

template <int dWidth, int aShift, int Endian>
class mapHandlerReadAddress : public mapHandlerRead<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapHandlerReadAddress(mapAddressSpace *space, uint32_t flags)
	: mapHandlerRead<dWidth, aShift, Endian>(space, flags)
	{ }

	~mapHandlerReadAddress() = default;

	inline void setAddressInfo(offs_t base, offs_t mask) {
		maskAddress = mask;
		baseAddress = base;
	}

protected:
	offs_t maskAddress;
	offs_t baseAddress;
};

template <int dWidth, int aShift, int Endian>
class mapHandlerWriteAddress : public mapHandlerWrite<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapHandlerWriteAddress(mapAddressSpace *space, uint32_t flags)
	: mapHandlerWrite<dWidth, aShift, Endian>(space, flags)
	{ }

	~mapHandlerWriteAddress() = default;

	inline void setAddressInfo(offs_t base, offs_t mask) {
		maskAddress = mask;
		baseAddress = base;
	}

protected:
	offs_t maskAddress;
	offs_t baseAddress;
};
