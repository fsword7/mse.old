/*
 * hedw3.cpp
 *
 *  Created on: Mar 5, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/map/hedw.h"
#include "emu/map/hedwi.h"

template class mapHandlerWriteDispatch<25, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<25, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<26, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<26, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<27, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<27, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<28, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<28, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<29, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<29, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<30, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<30, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<31, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<31, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<32, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<32, 0, 0, endianBig>;

template class mapHandlerWriteDispatch<25, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<25, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<26, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<26, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<27, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<27, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<28, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<28, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<29, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<29, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<30, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<30, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<31, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<31, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<32, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<32, 1, 0, endianBig>;

template class mapHandlerWriteDispatch<25, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<25, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<26, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<26, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<27, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<27, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<28, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<28, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<29, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<29, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<30, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<30, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<31, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<31, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<32, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<32, 1, -1, endianBig>;

template class mapHandlerWriteDispatch<25, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<25, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<26, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<26, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<27, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<27, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<28, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<28, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<29, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<29, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<30, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<30, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<31, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<31, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<32, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<32, 2, 0, endianBig>;

template class mapHandlerWriteDispatch<25, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<25, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<26, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<26, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<27, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<27, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<28, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<28, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<29, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<29, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<30, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<30, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<31, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<31, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<32, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<32, 2, -1, endianBig>;

template class mapHandlerWriteDispatch<25, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<25, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<26, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<26, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<27, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<27, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<28, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<28, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<29, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<29, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<30, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<30, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<31, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<31, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<32, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<32, 2, -2, endianBig>;

template class mapHandlerWriteDispatch<25, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<25, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<26, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<26, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<27, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<27, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<28, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<28, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<29, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<29, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<30, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<30, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<31, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<31, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<32, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<32, 3, 0, endianBig>;

template class mapHandlerWriteDispatch<25, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<25, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<26, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<26, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<27, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<27, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<28, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<28, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<29, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<29, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<30, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<30, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<31, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<31, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<32, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<32, 3, -1, endianBig>;

template class mapHandlerWriteDispatch<25, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<25, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<26, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<26, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<27, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<27, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<28, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<28, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<29, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<29, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<30, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<30, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<31, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<31, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<32, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<32, 3, -2, endianBig>;

template class mapHandlerWriteDispatch<25, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<25, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<26, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<26, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<27, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<27, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<28, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<28, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<29, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<29, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<30, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<30, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<31, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<31, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<32, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<32, 3, -3, endianBig>;
