/*
 * emumap_hed.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/map/hedr.h"
#include "emu/map/hedri.h"

template class mapHandlerReadDispatch< 1, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 1, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 2, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 2, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 3, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 3, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 4, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 4, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 5, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 5, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 6, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 6, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 7, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 7, 0, 0, endianBig>;
template class mapHandlerReadDispatch< 8, 0, 0, endianLittle>;
template class mapHandlerReadDispatch< 8, 0, 0, endianBig>;

template class mapHandlerReadDispatch< 1, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 1, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 2, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 2, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 3, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 3, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 4, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 4, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 5, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 5, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 6, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 6, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 7, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 7, 1, 0, endianBig>;
template class mapHandlerReadDispatch< 8, 1, 0, endianLittle>;
template class mapHandlerReadDispatch< 8, 1, 0, endianBig>;

template class mapHandlerReadDispatch< 1, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 1, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 2, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 2, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 3, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 3, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 4, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 4, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 5, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 5, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 6, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 6, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 7, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 7, 1, -1, endianBig>;
template class mapHandlerReadDispatch< 8, 1, -1, endianLittle>;
template class mapHandlerReadDispatch< 8, 1, -1, endianBig>;

template class mapHandlerReadDispatch< 2, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 2, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 3, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 3, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 4, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 4, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 5, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 5, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 6, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 6, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 7, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 7, 2, 0, endianBig>;
template class mapHandlerReadDispatch< 8, 2, 0, endianLittle>;
template class mapHandlerReadDispatch< 8, 2, 0, endianBig>;

template class mapHandlerReadDispatch< 2, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 2, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 3, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 3, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 4, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 4, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 5, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 5, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 6, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 6, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 7, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 7, 2, -1, endianBig>;
template class mapHandlerReadDispatch< 8, 2, -1, endianLittle>;
template class mapHandlerReadDispatch< 8, 2, -1, endianBig>;

template class mapHandlerReadDispatch< 2, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 2, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 3, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 3, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 4, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 4, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 5, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 5, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 6, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 6, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 7, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 7, 2, -2, endianBig>;
template class mapHandlerReadDispatch< 8, 2, -2, endianLittle>;
template class mapHandlerReadDispatch< 8, 2, -2, endianBig>;

template class mapHandlerReadDispatch< 3, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 3, 3, 0, endianBig>;
template class mapHandlerReadDispatch< 4, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 4, 3, 0, endianBig>;
template class mapHandlerReadDispatch< 5, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 5, 3, 0, endianBig>;
template class mapHandlerReadDispatch< 6, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 6, 3, 0, endianBig>;
template class mapHandlerReadDispatch< 7, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 7, 3, 0, endianBig>;
template class mapHandlerReadDispatch< 8, 3, 0, endianLittle>;
template class mapHandlerReadDispatch< 8, 3, 0, endianBig>;

template class mapHandlerReadDispatch< 3, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 3, 3, -1, endianBig>;
template class mapHandlerReadDispatch< 4, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 4, 3, -1, endianBig>;
template class mapHandlerReadDispatch< 5, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 5, 3, -1, endianBig>;
template class mapHandlerReadDispatch< 6, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 6, 3, -1, endianBig>;
template class mapHandlerReadDispatch< 7, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 7, 3, -1, endianBig>;
template class mapHandlerReadDispatch< 8, 3, -1, endianLittle>;
template class mapHandlerReadDispatch< 8, 3, -1, endianBig>;

template class mapHandlerReadDispatch< 3, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 3, 3, -2, endianBig>;
template class mapHandlerReadDispatch< 4, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 4, 3, -2, endianBig>;
template class mapHandlerReadDispatch< 5, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 5, 3, -2, endianBig>;
template class mapHandlerReadDispatch< 6, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 6, 3, -2, endianBig>;
template class mapHandlerReadDispatch< 7, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 7, 3, -2, endianBig>;
template class mapHandlerReadDispatch< 8, 3, -2, endianLittle>;
template class mapHandlerReadDispatch< 8, 3, -2, endianBig>;

template class mapHandlerReadDispatch< 3, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 3, 3, -3, endianBig>;
template class mapHandlerReadDispatch< 4, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 4, 3, -3, endianBig>;
template class mapHandlerReadDispatch< 5, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 5, 3, -3, endianBig>;
template class mapHandlerReadDispatch< 6, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 6, 3, -3, endianBig>;
template class mapHandlerReadDispatch< 7, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 7, 3, -3, endianBig>;
template class mapHandlerReadDispatch< 8, 3, -3, endianLittle>;
template class mapHandlerReadDispatch< 8, 3, -3, endianBig>;
