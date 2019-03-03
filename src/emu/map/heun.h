/*
 * emumap_heun.h
 *
 *  Created on: Feb 14, 2019
 *      Author: fswor
 */

#pragma once

template <int dWidth, int aShift, int Endian>
class mapReadHandlerUnmapped : public mapReadHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using rhe = mapReadHandlerEntry<dWidth, aShift, Endian>;

	mapReadHandlerUnmapped(mapAddressSpace *space)
	: mapReadHandlerEntry<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapReadHandlerUnmapped() = default;

	std::string name() const override;

	uintx_t read(offs_t offset, uintx_t mask) override;
};

template <int dWidth, int aShift, int Endian>
class mapWriteHandlerUnmapped : public mapWriteHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapWriteHandlerEntry<dWidth, aShift, Endian>;

	mapWriteHandlerUnmapped(mapAddressSpace *space)
	: mapWriteHandlerEntry<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapWriteHandlerUnmapped() = default;

	std::string name() const override;

	void write(offs_t offset, uintx_t data, uintx_t mask) override;
};


template <int dWidth, int aShift, int Endian>
class mapReadHandlerNop : public mapReadHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using rhe = mapReadHandlerEntry<dWidth, aShift, Endian>;

	mapReadHandlerNop(mapAddressSpace *space)
	: mapReadHandlerEntry<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapReadHandlerNop() = default;

	std::string name() const override;

	uintx_t read(offs_t offset, uintx_t mask) override;
};

template <int dWidth, int aShift, int Endian>
class mapWriteHandlerNop : public mapWriteHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapWriteHandlerEntry<dWidth, aShift, Endian>;

	mapWriteHandlerNop(mapAddressSpace *space)
	: mapWriteHandlerEntry<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapWriteHandlerNop() = default;

	std::string name() const override;

	void write(offs_t offset, uintx_t data, uintx_t mask) override;
};
