/*
 * hedwi.h
 *
 *  Created on: Mar 5, 2019
 *      Author: fswor
 */

#pragma once

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::range_cut_before(offs_t address, int start)
{
	while (--start >= 0) {
		if (int(lowBits) > -aShift && dispatch[start]->isDispatch()) {
			static_cast<mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian> *>(dispatch[start])->range_cut_before(address);
			break;
		}
		if (ranges[start].end <= address)
			break;
		ranges[start].end = address;
	}
}

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::range_cut_after(offs_t address, int start)
{
	while (++start < count) {
		if (int(lowBits) > -aShift && dispatch[start]->isDispatch()) {
			static_cast<mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian> *>(dispatch[start])->range_cut_after(address);
			break;
		}
		if (ranges[start].start <= address)
			break;
		ranges[start].start = address;
	}
}

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::populate_mirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
	offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler)
{
	cty.printf("%s: (W) %08X-%08X (%08X-%08X) - entry %08X mirror %08X  %p\n",
		name().c_str(), start, end, ostart, oend, entry, mirror, handler);

	auto cur = dispatch[entry];
	if (cur->isDispatch())
		cur->populate_mirror(cty, start, end, ostart, oend, mirror, handler);
	else {
		auto subDispatch = new mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian>(mapHandlerEntry::space, ranges[entry], cur);
		cur->unref();
		dispatch[entry] = subDispatch;
		subDispatch->populate_mirror(cty, start, end, ostart, oend, mirror, handler);
	}
}

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::populate_nomirror_subdispatch(const cty_t &cty, offs_t entry, offs_t start, offs_t end, offs_t ostart, offs_t oend,
	mapHandlerWrite<dWidth, aShift, Endian> *handler)
{
	cty.printf("%s: (W) %08X-%08X (%08X-%08X) - entry %08X  %p\n",
		name().c_str(), start, end, ostart, oend, entry, handler);

	auto cur = dispatch[entry];
	if (cur->isDispatch())
		cur->populate_nomirror(cty, start, end, ostart, oend, handler);
	else {
		auto subDispatch = new mapHandlerWriteDispatch<lowBits, dWidth, aShift, Endian>(mapHandlerEntry::space, ranges[entry], cur);
		cur->unref();
		dispatch[entry] = subDispatch;
		subDispatch->populate_nomirror(cty, start, end, ostart, oend, handler);
	}
}

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::populate_mirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
	offs_t mirror, mapHandlerWrite<dWidth, aShift, Endian> *handler)
{
	offs_t hmirror = mirror & highMask;
	offs_t lmirror = mirror & lowMask;

	cty.printf("%s: (R) High bits %d  low bits %d  bit count %d  count %d  mask %08X low %08X high %08X up %08X\n",
		name().c_str(), highBits, lowBits, bitCount, count, bitMask, lowMask, highMask, upMask);
	cty.printf("%s: (W) %08X-%08X (%08X-%08X) - mirror %08X %08X  %p\n",
		name().c_str(), start, end, ostart, oend, hmirror, lmirror, handler);

	if (lmirror != 0) {
		// Non-zero low mirror bits
		offs_t add = 1 + ~hmirror;
		offs_t offset = 0;
		offs_t baseEntry = start >> lowBits;
		start &= lowMask;
		end &= lowMask;

		do {
			if (offset != 0)
				handler->ref();
			populate_mirror_subdispatch(cty, (baseEntry | (offset >> lowBits)),
				start, end, (ostart | offset), (oend | offset), lmirror, handler);
			offset = (offset + add) & hmirror;
		} while (offset != 0);
	} else {
		// Zero low mirror bits
		offs_t add = 1 + ~hmirror;
		offs_t offset = 0;

		do {
			if (offset != 0)
				handler->ref();
			populate_nomirror(cty, (start | offset), (end | offset),
				(ostart | offset), (oend | offset), handler);
			offset = (offset + add) & hmirror;
		} while (offset != 0);
	}
}

template <int highBits, int dWidth, int aShift, int Endian>
void mapHandlerWriteDispatch<highBits, dWidth, aShift, Endian>::populate_nomirror(const cty_t &cty, offs_t start, offs_t end, offs_t ostart, offs_t oend,
	mapHandlerWrite<dWidth, aShift, Endian> *handler)
{
	offs_t sEntry = start >> lowBits;
	offs_t eEntry = end >> lowBits;

	cty.printf("%s: (R) High bits %d  low bits %d  bit count %d  count %d  mask %08X low %08X high %08X up %08X\n",
		name().c_str(), highBits, lowBits, bitCount, count, bitMask, lowMask, highMask, upMask);
	cty.printf("%s: (W) %08X-%08X (%08X-%08X) - entry %08X-%08X  %p\n",
		name().c_str(), start, end, ostart, oend, sEntry, eEntry, handler);

	range_cut_before(ostart-1, sEntry);
	range_cut_after(oend+1, eEntry);

	if (lowBits <= dWidth + aShift) {
		handler->ref(eEntry - sEntry);
		for (offs_t ent = sEntry; ent <= eEntry; ent++) {
			dispatch[ent]->unref();
			dispatch[ent] = handler;
			ranges[ent].set(ostart, oend);
		}
	} else if (sEntry == eEntry) {
		if (!(start & lowMask) && (end == lowMask) == lowMask) {
			dispatch[sEntry]->unref();
			dispatch[sEntry] = handler;
			ranges[sEntry].set(ostart, oend);
		} else
			populate_nomirror_subdispatch(cty, sEntry, start & lowMask, end & lowMask, ostart, oend, handler);
	} else {
		if (start & lowMask) {
			populate_nomirror_subdispatch(cty, sEntry, start & lowMask, lowMask, ostart, oend, handler);
			sEntry++;
			if (sEntry <= eEntry)
				handler->ref();
		}
		if ((start & lowMask) != lowMask) {
			populate_nomirror_subdispatch(cty, eEntry, 0, end & lowMask, ostart, oend, handler);
			sEntry--;
			if (sEntry <= eEntry)
				handler->ref();
		}
		if (sEntry == eEntry) {
			handler->ref(eEntry - sEntry);
			for (offs_t ent = sEntry; ent <= eEntry; ent++) {
				dispatch[ent]->unref();
				dispatch[ent] = handler;
				ranges[ent].set(ostart, oend);
			}
		}
	}
}
