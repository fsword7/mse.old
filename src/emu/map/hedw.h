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

//	void range_cut_before(offs_t address, int start = count)
//	{
//		while (--start >= 0) {
//			if (int(lowBits) > -aShift && dispatch[start]->isDispatch()) {
//				static_cast<mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian> *>(dispatch[start])->range_cut_before(address);
//				break;
//			}
//			if (ranges[start].end <= address)
//				break;
//			ranges[start].end = address;
//		}
//	}
//
//	void range_cut_after(offs_t address, int start = -1)
//	{
//		while (++start < count) {
//			if (int(lowBits) > -aShift && dispatch[start]->isDispatch()) {
//				static_cast<mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian> *>(dispatch[start])->range_cut_after(address);
//				break;
//			}
//			if (ranges[start].start <= address)
//				break;
//			ranges[start].start = address;
//		}
//	}
//
//	void populate_mirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
//		offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler)
//	{
//		auto cur = dispatch[entry];
//		if (cur->isDispatch())
//			cur->populate_mirror(cty, start, end, ostart, oend, mirror, handler);
//		else {
//			auto subDispatch = new mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian>(mapHandlerEntry::space, ranges[entry], cur);
//			cur->unref();
//			dispatch[entry] = subDispatch;
//			subDispatch->populate_mirror(cty, start, end, ostart, oend, mirror, handler);
//		}
//	}
//
//	void populate_nomirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
//		mapHandlerWrite<dWidth, aShift, Endian> *handler)
//	{
//		auto cur = dispatch[entry];
//		if (cur->isDispatch())
//			cur->populate_nomirror(cty, start, end, ostart, oend, handler);
//		else {
//			auto subDispatch = new mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian>(mapHandlerEntry::space, ranges[entry], cur);
//			cur->unref();
//			dispatch[entry] = subDispatch;
//			subDispatch->populate_nomirror(cty, start, end, ostart, oend, handler);
//		}
//	}
//
//	void populate_mirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
//		offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler) override
//	{
//		offs_t hmirror = mirror & highMask;
//		offs_t lmirror = mirror & lowMask;
//
//		if (lmirror != 0) {
//			// Non-zero low mirror bits
//			offs_t add = 1 + ~hmirror;
//			offs_t offset = 0;
//			offs_t baseEntry = start >> lowBits;
//			start &= lowMask;
//			end &= lowMask;
//
//			do {
//				if (offset != 0)
//					handler->ref();
//				populate_mirror_subdispatch(cty, (baseEntry | (offset >> lowBits)),
//					start, end, (ostart | offset), (oend | offset), lmirror, handler);
//				offset = (offset + add) & hmirror;
//			} while (offset != 0);
//		} else {
//			// Zero low mirror bits
//			offs_t add = 1 + ~hmirror;
//			offs_t offset = 0;
//
//			do {
//				if (offset != 0)
//					handler->ref();
//				populate_nomirror(cty, (start | offset), (end | offset),
//					(ostart | offset), (oend | offset), handler);
//				offset = (offset + add) & hmirror;
//			} while (offset != 0);
//		}
//	}
//
//	void populate_nomirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
//		mapHandlerWrite<dWidth, aShift, Endian> *handler) override
//	{
//		offs_t sEntry = start >> lowBits;
//		offs_t eEntry = end >> lowBits;
//
//		range_cut_before(ostart-1, sEntry);
//		range_cut_after(oend+1, eEntry);
//
//		if (lowBits <= dWidth + aShift) {
//			handler->ref(eEntry - sEntry);
//			for (offs_t ent = sEntry; ent <= eEntry; ent++) {
//				dispatch[ent]->unref();
//				dispatch[ent] = handler;
//				ranges[ent].set(ostart, oend);
//			}
//		} else if (sEntry == eEntry) {
//			if (!(start & lowMask) && (end == lowMask) == lowMask) {
//				dispatch[sEntry]->unref();
//				dispatch[sEntry] = handler;
//				ranges[sEntry].set(ostart, oend);
//			} else
//				populate_nomirror_subdispatch(cty, sEntry, start & lowMask, end & lowMask, ostart, oend, handler);
//		} else {
//			if (start & lowMask) {
//				populate_nomirror_subdispatch(cty, sEntry, start & lowMask, lowMask, ostart, oend, handler);
//				sEntry++;
//				if (sEntry <= eEntry)
//					handler->ref();
//			}
//			if ((start & lowMask) != lowMask) {
//				populate_nomirror_subdispatch(cty, eEntry, 0, end & lowMask, ostart, oend, handler);
//				sEntry--;
//				if (sEntry <= eEntry)
//					handler->ref();
//			}
//			if (sEntry == eEntry) {
//				handler->ref(eEntry - sEntry);
//				for (offs_t ent = sEntry; ent <= eEntry; ent++) {
//					dispatch[ent]->unref();
//					dispatch[ent] = handler;
//					ranges[ent].set(ostart, oend);
//				}
//			}
//		}
//	}

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
