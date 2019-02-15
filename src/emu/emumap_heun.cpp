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
std::string mapReadHandlerUnmapped<dWidth, aShift, Endian>::name()
{
	return "unmapped";
}

template <int dWidth, int aShift, endian_t Endian>
typename mapHandlerSize<dWidth>::uintx_t mapWriteHandlerUnmapped<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, endian_t Endian>
std::string mapWriteHandlerUnmapped<dWidth, aShift, Endian>::name()
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
std::string mapReadHandlerNop<dWidth, aShift, Endian>::name()
{
	return "nop";
}

template <int dWidth, int aShift, endian_t Endian>
typename mapHandlerSize<dWidth>::uintx_t mapWriteHandlerNop<dWidth, aShift, Endian>::write(offs_t offset, uintx_t data, uintx_t mask)
{

}

template <int dWidth, int aShift, endian_t Endian>
std::string mapWriteHandlerNop<dWidth, aShift, Endian>::name()
{
	return "nop";
}

// ************************************************************
