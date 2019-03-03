/*
 * emumap_he.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "lib/util/templates.h"

// Data width - types
template <int dWidth> struct mapHandlerSize { };
template<> struct mapHandlerSize<0> { using uintx_t = uint8_t; };
template<> struct mapHandlerSize<1> { using uintx_t = uint16_t; };
template<> struct mapHandlerSize<2> { using uintx_t = uint32_t; };
template<> struct mapHandlerSize<3> { using uintx_t = uint64_t; };

constexpr int mapHandlerDispatchLowBits(int highBits, int dWidth, int aShift)
{
	return	(highBits > 48) ? 48 :
			(highBits > 24) ? 24 :
			(highBits > 14) ? 14:
			dWidth + aShift;
}

// Generic read access
template<int dWidth, int aShift, int Endian, int tWidth, bool Aligned, typename T>
typename mapHandlerSize<tWidth>::uintx_t mapReadGeneric(T rop, offs_t address,
		typename mapHandlerSize<tWidth>::unitx_t mask)
{
	using targetType = typename mapHandlerSize<tWidth>::uintx_t;
	using nativeType = typename mapHandlerSize<dWidth>::uintx_t;

	constexpr uint32_t targetBytes = 1 << tWidth;
	constexpr uint32_t targetBits = 8 * targetBytes;
	constexpr uint32_t nativeBytes = 1 << dWidth;
	constexpr uint32_t nativeBits = 8 * nativeBytes;
	constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes << labs(aShift) : nativeBytes >> labs(aShift);
	constexpr uint32_t nativeMask = dWidth + aShift >= 0 ? (1 << (dWidth + aShift)) - 1 : 0;

	// For equals to native size and aligned
	if (nativeBytes == targetBytes && (Aligned || (address & nativeMask) == 0))
		return rop(address & ~nativeMask, mask);

}

// Generic write access
template<int dWidth, int aShift, int Endian, int tWidth, bool Aligned, typename T>
typename mapHandlerSize<tWidth>::uintx_t mapReadGeneric(T wop, offs_t address,
		typename mapHandlerSize<tWidth>::unitx_t data,
		typename mapHandlerSize<tWidth>::unitx_t mask)
{
	using targetType = typename mapHandlerSize<tWidth>::uintx_t;
	using nativeType = typename mapHandlerSize<dWidth>::uintx_t;

	constexpr uint32_t targetBytes = 1 << tWidth;
	constexpr uint32_t targetBits = 8 * targetBytes;
	constexpr uint32_t nativeBytes = 1 << dWidth;
	constexpr uint32_t nativeBits = 8 * nativeBytes;
	constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes << labs(aShift) : nativeBytes >> labs(aShift);
	constexpr uint32_t nativeMask = dWidth + aShift >= 0 ? (1 << (dWidth + aShift)) - 1 : 0;

	// For equals to native size and aligned
	if (nativeBytes == targetBytes && (Aligned || (address & nativeMask) == 0))
		return wop(address & ~nativeMask, data, mask);

}

class mapHandlerEntry {
public:

	static constexpr uint32_t heDispatch	= 0x00000001;
	static constexpr uint32_t heUnits		= 0x00000002;
	static constexpr uint32_t hePassThrough	= 0x00000004;

	static constexpr uint8_t heStart = 1;
	static constexpr uint8_t heEnd   = 2;

	mapHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: space(space), flags(flags), refCount(1)
	{ }

	virtual ~mapHandlerEntry() {}

	inline bool isDispatch() const { return flags & heDispatch; }
	inline bool isUnits() const { return flags & heUnits; }
	inline bool isPassThrough() const { return flags & hePassThrough; }
	inline uint32_t getFlags() const { return flags; }

	virtual std::string name() const = 0;

	inline void ref(int count = 1)
	{
		refCount += count;
	}

	inline void unref(int count = 1)
	{
		refCount -= count;
		if (refCount == 0)
			delete this;
	}

protected:
	mapAddressSpace		*space;
	uint32_t			flags;
	mutable uint32_t	refCount;

	struct range {
		offs_t start;
		offs_t end;

		inline void set(offs_t _start, offs_t _end)
		{
			start = _start;
			end = _end;
		}

		inline void intersect(offs_t _start, offs_t _end)
		{
			if (_start > start)
				start = _start;
			if (_end < end)
				end = _end;
		}
	};
};

template <int dWidth, int aShift, int Endian>
class mapReadHandlerEntry : public mapHandlerEntry
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapReadHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	virtual ~mapReadHandlerEntry() {}

	virtual uintx_t read(offs_t address, uintx_t mask) = 0;

	inline void populate(const cty_t &cty, offs_t address, offs_t &start, offs_t &end, offs_t mirror,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		if (mirror != 0)
			populate_mirror(start, end, start, end, mirror, handler);
		else
			populate_nomirror(start, end, start, end, handler);
	}

	virtual void populate_mirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend, offs_t mirror,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		cty.printf("%s: Populate (mirror) called on non-dispatching read class\n", name());
	}

	virtual void populate_nomirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		cty.printf("%s: Populate (nomirror) called on non-dispatching read class\n", name());
	}

};

template <int dWidth, int aShift, int Endian>
class mapWriteHandlerEntry : public mapHandlerEntry
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapWriteHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	virtual ~mapWriteHandlerEntry() {}

	virtual void write(offs_t address, uintx_t data, uintx_t mask) = 0;

	inline void populate(const cty_t &cty, offs_t address, offs_t &start, offs_t &end, offs_t mirror,
			mapWriteHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		if (mirror != 0)
			populate_mirror(start, end, start, end, mirror, handler);
		else
			populate_nomirror(start, end, start, end, handler);
	}

	virtual void populate_mirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend, offs_t mirror,
			mapWriteHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		cty.printf("%s: Populate (mirror) called on non-dispatching write class\n", name());
	}

	virtual void populate_nomirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
			mapWriteHandlerEntry<dWidth, aShift, Endian> *handler)
	{
		cty.printf("%s: Populate (nomirror) called on non-dispatching write class\n", name());
	}

};
