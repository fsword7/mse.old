/*
 * emumap_hed.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/map/hedr.h"

template class mapReadHandlerDispatch< 1, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 1, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 2, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 2, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 3, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 3, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 4, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 4, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 5, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 5, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 6, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 6, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 7, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 7, 0, 0, endianBig>;
template class mapReadHandlerDispatch< 8, 0, 0, endianLittle>;
template class mapReadHandlerDispatch< 8, 0, 0, endianBig>;
