/*
 * emumap_heun.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/emumap.h"

template <int dWidth, int aShift, endian_t Endian>
typename mapHandlerSize<dWidth>::uintx_t mapReadHandlerUnmapped<dWidth, aShift, Endian>::read(offs_t offset, uintx_t mask)
{
	return rhe::space->unmap();
}

template <int dWidth, int aShift, endian_t Endian>
std::string mapReadHandlerUnmapped<dWidth, aShift, Endian>::name() const
{
	return "unmapped";
}

template <int dWidth, int aShift, endian_t Endian>
void mapWriteHandlerUnmapped<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, endian_t Endian>
std::string mapWriteHandlerUnmapped<dWidth, aShift, Endian>::name() const
{
	return "unmapped";
}

// ************************************************************

template <int dWidth, int aShift, endian_t Endian>
typename mapHandlerSize<dWidth>::uintx_t mapReadHandlerNop<dWidth, aShift, Endian>::read(offs_t offset, uintx_t mask)
{
	return rhe::space->unmap();
}

template <int dWidth, int aShift, endian_t Endian>
std::string mapReadHandlerNop<dWidth, aShift, Endian>::name() const
{
	return "nop";
}

template <int dWidth, int aShift, endian_t Endian>
void mapWriteHandlerNop<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, endian_t Endian>
std::string mapWriteHandlerNop<dWidth, aShift, Endian>::name() const
{
	return "nop";
}

// ************************************************************

template class mapReadHandlerUnmapped<0, 0, endianLittle>;
template class mapReadHandlerUnmapped<0, 0, endianBig>;

template class mapWriteHandlerUnmapped<0, 0, endianLittle>;
template class mapWriteHandlerUnmapped<0, 0, endianBig>;

template class mapReadHandlerNop<0, 0, endianLittle>;
template class mapReadHandlerNop<0, 0, endianBig>;

template class mapWriteHandlerNop<0, 0, endianLittle>;
template class mapWriteHandlerNop<0, 0, endianBig>;
