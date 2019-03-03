/*
 * emumap_hedr.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"

//template<int highBits, int dWidth, int aShift, endian_t Endian>
//mapReadHandlerDispatch<highBits, dWidth, aShift, Endian>::mapReadHandlerDispatch(mapAddressSpace *space,
//		const mapHandlerEntry::range &init,
//		mapReadHandlerEntry<dWidth, aShift, Endian> *handler)
//: mapReadHandlerEntry<dWidth, aShift, Endian>(space, mapHandlerEntry::heDispatch)
//{
//	if (handler == nullptr)
//		handler = space->getReadUnmap<dWidth, aShift, Endian>();
//
//	for (unsigned int idx = 0; idx != count; idx++) {
//		dispatch[idx] = handler;
//		ranges[idx] = init;
//	}
//}
//
//
//template<int highBits, int dWidth, int aShift, endian_t Endian>
//mapReadHandlerDispatch<highBits, dWidth, aShift, Endian>::~mapReadHandlerDispatch()
//{
//	for (unsigned int idx = 0; idx != count; idx++)
//		dispatch[idx]->unref();
//}
//
//template<int highBits, int dWidth, int aShift, endian_t Endian>
//typename mapHandlerSize<dWidth>::uintx_t mapReadHandlerDispatch<highBits, dWidth, aShift, Endian>::read(offs_t offset, uintx_t mask)
//{
//	return dispatch[(offset >> lowBits) & bitMask]->read(offset, mask);
//}
