/*
 * emumap_heun.h
 *
 *  Created on: Feb 14, 2019
 *      Author: fswor
 */

#pragma once

template <int dWidth, int aShift, int Endian>
class mapHandlerReadUnmapped : public mapHandlerRead<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using rhe = mapHandlerRead<dWidth, aShift, Endian>;

	mapHandlerReadUnmapped(mapAddressSpace *space)
	: mapHandlerRead<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapHandlerReadUnmapped() = default;

	std::string name() const override;

	uintx_t read(offs_t offset, uintx_t mask) override;
};

template <int dWidth, int aShift, int Endian>
class mapHandlerWriteUnmapped : public mapHandlerWrite<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapHandlerWrite<dWidth, aShift, Endian>;

	mapHandlerWriteUnmapped(mapAddressSpace *space)
	: mapHandlerWrite<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapHandlerWriteUnmapped() = default;

	std::string name() const override;

	void write(offs_t offset, uintx_t data, uintx_t mask) override;
};


template <int dWidth, int aShift, int Endian>
class mapHandlerReadNop : public mapHandlerRead<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using rhe = mapHandlerRead<dWidth, aShift, Endian>;

	mapHandlerReadNop(mapAddressSpace *space)
	: mapHandlerRead<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapHandlerReadNop() = default;

	std::string name() const override;

	uintx_t read(offs_t offset, uintx_t mask) override;
};

template <int dWidth, int aShift, int Endian>
class mapHandlerWriteNop : public mapHandlerWrite<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapHandlerWrite<dWidth, aShift, Endian>;

	mapHandlerWriteNop(mapAddressSpace *space)
	: mapHandlerWrite<dWidth, aShift, Endian>(space, 0)
	{ }

	~mapHandlerWriteNop() = default;

	std::string name() const override;

	void write(offs_t offset, uintx_t data, uintx_t mask) override;
};
