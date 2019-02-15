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
			mapReadHandlerEntry<dWidth, aShift, Endian> *handler)
	: mapReadHandlerEntry<dWidth, aShift, Endian>(space, mapHandlerEntry::heDispatch)
	{
//		if (handler == nullptr)
//			handler = space->getReadUnmap<dWidth, aShift, Endian>();

		for (unsigned int idx = 0; idx != count; idx++) {
			dispatch[idx] = handler;
			ranges[idx] = init;
		}
	}

	~mapReadHandlerDispatch()
	{
		for (unsigned int idx = 0; idx != count; idx++)
			dispatch[idx]->unref();
	}

	uintx_t read(offs_t offset, uintx_t mask) override
	{
		return dispatch[(offset >> lowBits) & bitMask]->read(offset, mask);
	}

protected:
	static constexpr uint32_t lowBits = mapHandlerDispatchLowBits(hBits, dWidth, aShift);
	static constexpr uint32_t bitCount = hBits > lowBits ? hBits - lowBits : 0;
	static constexpr uint32_t count = 1 << bitCount;
	static constexpr offs_t   bitMask = makeBitMask<offs_t>(bitCount);
	static constexpr offs_t   lowMask = makeBitMask<offs_t>(lowBits);
	static constexpr offs_t   highMask = makeBitMask<offs_t>(hBits) - lowMask;
	static constexpr offs_t   upMask = makeBitMask<offs_t>(hBits);

	mapReadHandlerEntry<dWidth, aShift, Endian> *dispatch[count];
	mapHandlerEntry::range ranges[count];

private:

};
