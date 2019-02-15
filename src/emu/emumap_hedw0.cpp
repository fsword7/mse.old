/*
 * emumap_hedw0.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/emumap.h"
#include "emu/emumap_hedw.h"

template class mapWriteHandlerDispatch< 1, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 1, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 2, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 2, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 3, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 3, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 4, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 4, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 5, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 5, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 6, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 6, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 7, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 7, 0, 0, endianBig>;
template class mapWriteHandlerDispatch< 8, 0, 0, endianLittle>;
template class mapWriteHandlerDispatch< 8, 0, 0, endianBig>;




