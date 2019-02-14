/*
 * emumap_he.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Tim Stark
 */

#pragma once

template <typename T>
constexpr std::enable_if_t<std::is_signed<T>::value, T> labs(T v) noexcept
{
	return (v < T(0)) ? -v : v;
}

// Data width - types
template <int dWidth> struct mapHandlerSize { };
template<> struct mapHandlerSize<0> { using uintx_t = uint8_t; };
template<> struct mapHandlerSize<1> { using uintx_t = uint16_t; };
template<> struct mapHandlerSize<2> { using uintx_t = uint32_t; };
template<> struct mapHandlerSize<3> { using uintx_t = uint64_t; };

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
	mapHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: space(space), flags(flags), refCount(1)
	{ }

	virtual ~mapHandlerEntry();

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

template <int dWidth, int aShift, endian_t Endian>
class mapReadHandlerEntry : public mapHandlerEntry
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapReadHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	virtual ~mapReadHandlerEntry();

	virtual uintx_t read(offs_t address, uintx_t mask) = 0;
};

template <int dWidth, int aShift, endian_t Endian>
class mapWriteHandlerEntry : public mapHandlerEntry
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;

	mapWriteHandlerEntry(mapAddressSpace *space, uint32_t flags)
	: mapHandlerEntry(space, flags) {}
	virtual ~mapWriteHandlerEntry();

	virtual uintx_t write(offs_t address, uintx_t data, uintx_t mask) = 0;
};
