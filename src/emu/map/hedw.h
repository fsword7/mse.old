/*
 * emumap_hedw.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#pragma once

template <int highBits, int dWidth, int aShift, int Endian>
class mapHandlerWriteDispatch : public mapHandlerWrite<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using whe = mapHandlerWrite<dWidth, aShift, Endian>;

	mapHandlerWriteDispatch(mapAddressSpace *space, const mapHandlerEntry::range &init,
			mapHandlerWrite<dWidth, aShift, Endian> *handler)
	: mapHandlerWrite<dWidth, aShift, Endian>(space, mapHandlerEntry::heDispatch)
	{
		if (handler == nullptr)
			handler = space->getWriteUnmap<dWidth, aShift, Endian>();

		for (unsigned int idx = 0; idx != count; idx++) {
			dispatch[idx] = handler;
			ranges[idx] = init;
		}
	}

	~mapHandlerWriteDispatch()
	{
		for (unsigned int idx = 0; idx != count; idx++)
			dispatch[idx]->unref();
	}

	std::string name() const override
	{
		return "dispatch";
	}

	void write(offs_t offset, uintx_t data, uintx_t mask) override
	{
		dispatch[(offset >> lowBits) & bitMask]->write(offset, data, mask);
	}

	void range_cut_before(offs_t address, int start = count);
	void range_cut_after(offs_t address, int start = -1);
	void populate_mirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
		offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler);
	void populate_nomirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
		mapHandlerWrite<dWidth, aShift, Endian> *handler);

	void populate_mirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
		offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler) override;
	void populate_nomirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
		mapHandlerWrite<dWidth, aShift, Endian> *handler) override;

protected:
	static constexpr uint32_t lowBits = mapHandlerDispatchLowBits(highBits, dWidth, aShift);
	static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
	static constexpr uint32_t count = 1 << bitCount;
	static constexpr offs_t   bitMask = makeBitMask<offs_t>(bitCount);
	static constexpr offs_t   lowMask = makeBitMask<offs_t>(lowBits);
	static constexpr offs_t   highMask = makeBitMask<offs_t>(highBits) - lowMask;
	static constexpr offs_t   upMask = makeBitMask<offs_t>(highBits);

	mapHandlerWrite<dWidth, aShift, Endian> *dispatch[count];
	mapHandlerEntry::range ranges[count];

private:

};
