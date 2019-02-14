/*
 * emumap_hedw.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#pragma once

template <int highBits, int dWidth, int aShift, endian_t Endian>
class mapWriteHandlerDispatch : public mapWriteHandlerEntry<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapWriteHandlerEntry<dWidth, aShift, Endian>;

	mapWriteHandlerDispatch(mapAddressSpace *space, const mapHandlerEntry::range &init,
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler);
	~mapWriteHandlerDispatch();

	void write(offs_t address, uintx_t data, uintx_t mask) override;

protected:
	static constexpr uint32_t lowBits = mapHandlerDispatchLowBits(highBits, dWidth, aShift);
	static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
	static constexpr uint32_t count = 1 << bitCount;
	static constexpr offs_t   bitMask = makeBitMask<offs_t>(bitCount);
	static constexpr offs_t   lowMask = makeBitMask<offs_t>(lowBits);
	static constexpr offs_t   highMask = makeBitMask<offs_t>(highBits) - lowMask;
	static constexpr offs_t   upMask = makeBitMask<offs_t>(highBits);

	mapWriteHandlerEntry<dWidth, aShift, Endian> *dispatch[count];
	mapHandlerEntry::range ranges[count];

private:


};
