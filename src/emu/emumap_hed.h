/*
 * emumap_hed.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#pragma once

template <int hBits, int dWidth, int aShift, endian_t Endian>
class mapReadHandlerDispatch : public mapReadHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using rhe = mapReadHandlerEntry<dWidth, aShift, Endian>;

	mapReadHandlerDispatch(mapAddressSpace *space, const mapHandlerEntry::range &init,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler);
	~mapReadHandlerDispatch();

	uintx_t read(offs_t address, uintx_t mask) override;

};

template <int hBits, int dWidth, int aShift, endian_t Endian>
class mapWriteHandlerDispatch : public mapWriteHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapWriteHandlerEntry<dWidth, aShift, Endian>;

	mapWriteHandlerDispatch(mapAddressSpace *space, const mapHandlerEntry::range &init,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler);
	~mapWriteHandlerDispatch();

	void write(offs_t address, uintx_t data, uintx_t mask) override;

};
