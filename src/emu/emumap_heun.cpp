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
template class mapReadHandlerUnmapped<1, 0, endianLittle>;
template class mapReadHandlerUnmapped<1, 0, endianBig>;
template class mapReadHandlerUnmapped<1, -1, endianLittle>;
template class mapReadHandlerUnmapped<1, -1, endianBig>;
template class mapReadHandlerUnmapped<2, 0, endianLittle>;
template class mapReadHandlerUnmapped<2, 0, endianBig>;
template class mapReadHandlerUnmapped<2, -1, endianLittle>;
template class mapReadHandlerUnmapped<2, -1, endianBig>;
template class mapReadHandlerUnmapped<2, -2, endianLittle>;
template class mapReadHandlerUnmapped<2, -2, endianBig>;
template class mapReadHandlerUnmapped<3, 0, endianLittle>;
template class mapReadHandlerUnmapped<3, 0, endianBig>;
template class mapReadHandlerUnmapped<3, -1, endianLittle>;
template class mapReadHandlerUnmapped<3, -1, endianBig>;
template class mapReadHandlerUnmapped<3, -2, endianLittle>;
template class mapReadHandlerUnmapped<3, -2, endianBig>;
template class mapReadHandlerUnmapped<3, -3, endianLittle>;
template class mapReadHandlerUnmapped<3, -3, endianBig>;

template class mapWriteHandlerUnmapped<0, 0, endianLittle>;
template class mapWriteHandlerUnmapped<0, 0, endianBig>;
template class mapWriteHandlerUnmapped<1, 0, endianLittle>;
template class mapWriteHandlerUnmapped<1, 0, endianBig>;
template class mapWriteHandlerUnmapped<1, -1, endianLittle>;
template class mapWriteHandlerUnmapped<1, -1, endianBig>;
template class mapWriteHandlerUnmapped<2, 0, endianLittle>;
template class mapWriteHandlerUnmapped<2, 0, endianBig>;
template class mapWriteHandlerUnmapped<2, -1, endianLittle>;
template class mapWriteHandlerUnmapped<2, -1, endianBig>;
template class mapWriteHandlerUnmapped<2, -2, endianLittle>;
template class mapWriteHandlerUnmapped<2, -2, endianBig>;
template class mapWriteHandlerUnmapped<3, 0, endianLittle>;
template class mapWriteHandlerUnmapped<3, 0, endianBig>;
template class mapWriteHandlerUnmapped<3, -1, endianLittle>;
template class mapWriteHandlerUnmapped<3, -1, endianBig>;
template class mapWriteHandlerUnmapped<3, -2, endianLittle>;
template class mapWriteHandlerUnmapped<3, -2, endianBig>;
template class mapWriteHandlerUnmapped<3, -3, endianLittle>;
template class mapWriteHandlerUnmapped<3, -3, endianBig>;

template class mapReadHandlerNop<0, 0, endianLittle>;
template class mapReadHandlerNop<0, 0, endianBig>;
template class mapReadHandlerNop<1, 0, endianLittle>;
template class mapReadHandlerNop<1, 0, endianBig>;
template class mapReadHandlerNop<1, -1, endianLittle>;
template class mapReadHandlerNop<1, -1, endianBig>;
template class mapReadHandlerNop<2, 0, endianLittle>;
template class mapReadHandlerNop<2, 0, endianBig>;
template class mapReadHandlerNop<2, -1, endianLittle>;
template class mapReadHandlerNop<2, -1, endianBig>;
template class mapReadHandlerNop<2, -2, endianLittle>;
template class mapReadHandlerNop<2, -2, endianBig>;
template class mapReadHandlerNop<3, 0, endianLittle>;
template class mapReadHandlerNop<3, 0, endianBig>;
template class mapReadHandlerNop<3, -1, endianLittle>;
template class mapReadHandlerNop<3, -1, endianBig>;
template class mapReadHandlerNop<3, -2, endianLittle>;
template class mapReadHandlerNop<3, -2, endianBig>;
template class mapReadHandlerNop<3, -3, endianLittle>;
template class mapReadHandlerNop<3, -3, endianBig>;

template class mapWriteHandlerNop<0, 0, endianLittle>;
template class mapWriteHandlerNop<0, 0, endianBig>;
template class mapWriteHandlerNop<1, 0, endianLittle>;
template class mapWriteHandlerNop<1, 0, endianBig>;
template class mapWriteHandlerNop<1, -1, endianLittle>;
template class mapWriteHandlerNop<1, -1, endianBig>;
template class mapWriteHandlerNop<2, 0, endianLittle>;
template class mapWriteHandlerNop<2, 0, endianBig>;
template class mapWriteHandlerNop<2, -1, endianLittle>;
template class mapWriteHandlerNop<2, -1, endianBig>;
template class mapWriteHandlerNop<2, -2, endianLittle>;
template class mapWriteHandlerNop<2, -2, endianBig>;
template class mapWriteHandlerNop<3, 0, endianLittle>;
template class mapWriteHandlerNop<3, 0, endianBig>;
template class mapWriteHandlerNop<3, -1, endianLittle>;
template class mapWriteHandlerNop<3, -1, endianBig>;
template class mapWriteHandlerNop<3, -2, endianLittle>;
template class mapWriteHandlerNop<3, -2, endianBig>;
template class mapWriteHandlerNop<3, -3, endianLittle>;
template class mapWriteHandlerNop<3, -3, endianBig>;
