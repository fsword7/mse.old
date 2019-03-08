/*
 * emumap_heun.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"

template <int dWidth, int aShift, int Endian>
typename mapHandlerSize<dWidth>::uintx_t mapHandlerReadUnmapped<dWidth, aShift, Endian>::read(offs_t offset, uintx_t mask)
{
//	printf("%s: Data width=%d Address=%08X Mask=%08X ** Unmapped! **\n",
//		name().c_str(), dWidth, offset, mask);
	return rhe::space->unmap();
}

template <int dWidth, int aShift, int Endian>
std::string mapHandlerReadUnmapped<dWidth, aShift, Endian>::name() const
{
	return "unmapped";
}

template <int dWidth, int aShift, int Endian>
void mapHandlerWriteUnmapped<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, int Endian>
std::string mapHandlerWriteUnmapped<dWidth, aShift, Endian>::name() const
{
	return "unmapped";
}

// ************************************************************

template <int dWidth, int aShift, int Endian>
typename mapHandlerSize<dWidth>::uintx_t mapHandlerReadNop<dWidth, aShift, Endian>::read(offs_t offset, uintx_t mask)
{
	return rhe::space->unmap();
}

template <int dWidth, int aShift, int Endian>
std::string mapHandlerReadNop<dWidth, aShift, Endian>::name() const
{
	return "nop";
}

template <int dWidth, int aShift, int Endian>
void mapHandlerWriteNop<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, int Endian>
std::string mapHandlerWriteNop<dWidth, aShift, Endian>::name() const
{
	return "nop";
}

// ************************************************************

template class mapHandlerReadUnmapped<0, 0, endianLittle>;
template class mapHandlerReadUnmapped<0, 0, endianBig>;
template class mapHandlerReadUnmapped<1, 0, endianLittle>;
template class mapHandlerReadUnmapped<1, 0, endianBig>;
template class mapHandlerReadUnmapped<1, -1, endianLittle>;
template class mapHandlerReadUnmapped<1, -1, endianBig>;
template class mapHandlerReadUnmapped<2, 0, endianLittle>;
template class mapHandlerReadUnmapped<2, 0, endianBig>;
template class mapHandlerReadUnmapped<2, -1, endianLittle>;
template class mapHandlerReadUnmapped<2, -1, endianBig>;
template class mapHandlerReadUnmapped<2, -2, endianLittle>;
template class mapHandlerReadUnmapped<2, -2, endianBig>;
template class mapHandlerReadUnmapped<3, 0, endianLittle>;
template class mapHandlerReadUnmapped<3, 0, endianBig>;
template class mapHandlerReadUnmapped<3, -1, endianLittle>;
template class mapHandlerReadUnmapped<3, -1, endianBig>;
template class mapHandlerReadUnmapped<3, -2, endianLittle>;
template class mapHandlerReadUnmapped<3, -2, endianBig>;
template class mapHandlerReadUnmapped<3, -3, endianLittle>;
template class mapHandlerReadUnmapped<3, -3, endianBig>;

template class mapHandlerWriteUnmapped<0, 0, endianLittle>;
template class mapHandlerWriteUnmapped<0, 0, endianBig>;
template class mapHandlerWriteUnmapped<1, 0, endianLittle>;
template class mapHandlerWriteUnmapped<1, 0, endianBig>;
template class mapHandlerWriteUnmapped<1, -1, endianLittle>;
template class mapHandlerWriteUnmapped<1, -1, endianBig>;
template class mapHandlerWriteUnmapped<2, 0, endianLittle>;
template class mapHandlerWriteUnmapped<2, 0, endianBig>;
template class mapHandlerWriteUnmapped<2, -1, endianLittle>;
template class mapHandlerWriteUnmapped<2, -1, endianBig>;
template class mapHandlerWriteUnmapped<2, -2, endianLittle>;
template class mapHandlerWriteUnmapped<2, -2, endianBig>;
template class mapHandlerWriteUnmapped<3, 0, endianLittle>;
template class mapHandlerWriteUnmapped<3, 0, endianBig>;
template class mapHandlerWriteUnmapped<3, -1, endianLittle>;
template class mapHandlerWriteUnmapped<3, -1, endianBig>;
template class mapHandlerWriteUnmapped<3, -2, endianLittle>;
template class mapHandlerWriteUnmapped<3, -2, endianBig>;
template class mapHandlerWriteUnmapped<3, -3, endianLittle>;
template class mapHandlerWriteUnmapped<3, -3, endianBig>;

template class mapHandlerReadNop<0, 0, endianLittle>;
template class mapHandlerReadNop<0, 0, endianBig>;
template class mapHandlerReadNop<1, 0, endianLittle>;
template class mapHandlerReadNop<1, 0, endianBig>;
template class mapHandlerReadNop<1, -1, endianLittle>;
template class mapHandlerReadNop<1, -1, endianBig>;
template class mapHandlerReadNop<2, 0, endianLittle>;
template class mapHandlerReadNop<2, 0, endianBig>;
template class mapHandlerReadNop<2, -1, endianLittle>;
template class mapHandlerReadNop<2, -1, endianBig>;
template class mapHandlerReadNop<2, -2, endianLittle>;
template class mapHandlerReadNop<2, -2, endianBig>;
template class mapHandlerReadNop<3, 0, endianLittle>;
template class mapHandlerReadNop<3, 0, endianBig>;
template class mapHandlerReadNop<3, -1, endianLittle>;
template class mapHandlerReadNop<3, -1, endianBig>;
template class mapHandlerReadNop<3, -2, endianLittle>;
template class mapHandlerReadNop<3, -2, endianBig>;
template class mapHandlerReadNop<3, -3, endianLittle>;
template class mapHandlerReadNop<3, -3, endianBig>;

template class mapHandlerWriteNop<0, 0, endianLittle>;
template class mapHandlerWriteNop<0, 0, endianBig>;
template class mapHandlerWriteNop<1, 0, endianLittle>;
template class mapHandlerWriteNop<1, 0, endianBig>;
template class mapHandlerWriteNop<1, -1, endianLittle>;
template class mapHandlerWriteNop<1, -1, endianBig>;
template class mapHandlerWriteNop<2, 0, endianLittle>;
template class mapHandlerWriteNop<2, 0, endianBig>;
template class mapHandlerWriteNop<2, -1, endianLittle>;
template class mapHandlerWriteNop<2, -1, endianBig>;
template class mapHandlerWriteNop<2, -2, endianLittle>;
template class mapHandlerWriteNop<2, -2, endianBig>;
template class mapHandlerWriteNop<3, 0, endianLittle>;
template class mapHandlerWriteNop<3, 0, endianBig>;
template class mapHandlerWriteNop<3, -1, endianLittle>;
template class mapHandlerWriteNop<3, -1, endianBig>;
template class mapHandlerWriteNop<3, -2, endianLittle>;
template class mapHandlerWriteNop<3, -2, endianBig>;
template class mapHandlerWriteNop<3, -3, endianLittle>;
template class mapHandlerWriteNop<3, -3, endianBig>;
