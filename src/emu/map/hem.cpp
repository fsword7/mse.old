/*
 * hem.cpp - memory access handlers
 *
 *  Created on: Mar 4, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/map/map.h"
#include "emu/map/hea.h"
#include "emu/map/hem.h"

//template <> void mapHandlerWriteMemory<0, 0, endianLittle>::write(offs_t offset, uint8_t data, uint8_t mask)
//{
//	baseData[(offset - inh::baseAddress) & inh::maskAddress] = data;
//}
//
//template <> void mapHandlerWriteMemory<0, 0, endianBig>::write(offs_t offset, uint8_t data, uint8_t mask)
//{
//	baseData[(offset - inh::baseAddress) & inh::maskAddress] = data;
//}

template class mapHandlerReadMemory<0, 0, endianLittle>;
template class mapHandlerReadMemory<0, 0, endianBig>;
template class mapHandlerReadMemory<1, 0, endianLittle>;
template class mapHandlerReadMemory<1, 0, endianBig>;
template class mapHandlerReadMemory<1, -1, endianLittle>;
template class mapHandlerReadMemory<1, -1, endianBig>;
template class mapHandlerReadMemory<2, 0, endianLittle>;
template class mapHandlerReadMemory<2, 0, endianBig>;
template class mapHandlerReadMemory<2, -1, endianLittle>;
template class mapHandlerReadMemory<2, -1, endianBig>;
template class mapHandlerReadMemory<2, -2, endianLittle>;
template class mapHandlerReadMemory<2, -2, endianBig>;
template class mapHandlerReadMemory<3, 0, endianLittle>;
template class mapHandlerReadMemory<3, 0, endianBig>;
template class mapHandlerReadMemory<3, -1, endianLittle>;
template class mapHandlerReadMemory<3, -1, endianBig>;
template class mapHandlerReadMemory<3, -2, endianLittle>;
template class mapHandlerReadMemory<3, -2, endianBig>;
template class mapHandlerReadMemory<3, -3, endianLittle>;
template class mapHandlerReadMemory<3, -3, endianBig>;

template class mapHandlerWriteMemory<0, 0, endianLittle>;
template class mapHandlerWriteMemory<0, 0, endianBig>;
template class mapHandlerWriteMemory<1, 0, endianLittle>;
template class mapHandlerWriteMemory<1, 0, endianBig>;
template class mapHandlerWriteMemory<1, -1, endianLittle>;
template class mapHandlerWriteMemory<1, -1, endianBig>;
template class mapHandlerWriteMemory<2, 0, endianLittle>;
template class mapHandlerWriteMemory<2, 0, endianBig>;
template class mapHandlerWriteMemory<2, -1, endianLittle>;
template class mapHandlerWriteMemory<2, -1, endianBig>;
template class mapHandlerWriteMemory<2, -2, endianLittle>;
template class mapHandlerWriteMemory<2, -2, endianBig>;
template class mapHandlerWriteMemory<3, 0, endianLittle>;
template class mapHandlerWriteMemory<3, 0, endianBig>;
template class mapHandlerWriteMemory<3, -1, endianLittle>;
template class mapHandlerWriteMemory<3, -1, endianBig>;
template class mapHandlerWriteMemory<3, -2, endianLittle>;
template class mapHandlerWriteMemory<3, -2, endianBig>;
template class mapHandlerWriteMemory<3, -3, endianLittle>;
template class mapHandlerWriteMemory<3, -3, endianBig>;

template class mapHandlerReadMemoryBank<0, 0, endianLittle>;
template class mapHandlerReadMemoryBank<0, 0, endianBig>;
template class mapHandlerReadMemoryBank<1, 0, endianLittle>;
template class mapHandlerReadMemoryBank<1, 0, endianBig>;
template class mapHandlerReadMemoryBank<1, -1, endianLittle>;
template class mapHandlerReadMemoryBank<1, -1, endianBig>;
template class mapHandlerReadMemoryBank<2, 0, endianLittle>;
template class mapHandlerReadMemoryBank<2, 0, endianBig>;
template class mapHandlerReadMemoryBank<2, -1, endianLittle>;
template class mapHandlerReadMemoryBank<2, -1, endianBig>;
template class mapHandlerReadMemoryBank<2, -2, endianLittle>;
template class mapHandlerReadMemoryBank<2, -2, endianBig>;
template class mapHandlerReadMemoryBank<3, 0, endianLittle>;
template class mapHandlerReadMemoryBank<3, 0, endianBig>;
template class mapHandlerReadMemoryBank<3, -1, endianLittle>;
template class mapHandlerReadMemoryBank<3, -1, endianBig>;
template class mapHandlerReadMemoryBank<3, -2, endianLittle>;
template class mapHandlerReadMemoryBank<3, -2, endianBig>;
template class mapHandlerReadMemoryBank<3, -3, endianLittle>;
template class mapHandlerReadMemoryBank<3, -3, endianBig>;

template class mapHandlerWriteMemoryBank<0, 0, endianLittle>;
template class mapHandlerWriteMemoryBank<0, 0, endianBig>;
template class mapHandlerWriteMemoryBank<1, 0, endianLittle>;
template class mapHandlerWriteMemoryBank<1, 0, endianBig>;
template class mapHandlerWriteMemoryBank<1, -1, endianLittle>;
template class mapHandlerWriteMemoryBank<1, -1, endianBig>;
template class mapHandlerWriteMemoryBank<2, 0, endianLittle>;
template class mapHandlerWriteMemoryBank<2, 0, endianBig>;
template class mapHandlerWriteMemoryBank<2, -1, endianLittle>;
template class mapHandlerWriteMemoryBank<2, -1, endianBig>;
template class mapHandlerWriteMemoryBank<2, -2, endianLittle>;
template class mapHandlerWriteMemoryBank<2, -2, endianBig>;
template class mapHandlerWriteMemoryBank<3, 0, endianLittle>;
template class mapHandlerWriteMemoryBank<3, 0, endianBig>;
template class mapHandlerWriteMemoryBank<3, -1, endianLittle>;
template class mapHandlerWriteMemoryBank<3, -1, endianBig>;
template class mapHandlerWriteMemoryBank<3, -2, endianLittle>;
template class mapHandlerWriteMemoryBank<3, -2, endianBig>;
template class mapHandlerWriteMemoryBank<3, -3, endianLittle>;
template class mapHandlerWriteMemoryBank<3, -3, endianBig>;
