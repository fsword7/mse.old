/*
 * hedw2.cpp
 *
 *  Created on: Mar 5, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/map/hedw.h"
#include "emu/map/hedwi.h"

template class mapHandlerWriteDispatch<17, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<17, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<18, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<18, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<19, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<19, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<20, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<20, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<21, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<21, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<22, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<22, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<23, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<23, 0, 0, endianBig>;
template class mapHandlerWriteDispatch<24, 0, 0, endianLittle>;
template class mapHandlerWriteDispatch<24, 0, 0, endianBig>;

template class mapHandlerWriteDispatch<17, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<17, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<18, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<18, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<19, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<19, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<20, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<20, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<21, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<21, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<22, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<22, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<23, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<23, 1, 0, endianBig>;
template class mapHandlerWriteDispatch<24, 1, 0, endianLittle>;
template class mapHandlerWriteDispatch<24, 1, 0, endianBig>;

template class mapHandlerWriteDispatch<17, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<17, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<18, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<18, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<19, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<19, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<20, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<20, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<21, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<21, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<22, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<22, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<23, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<23, 1, -1, endianBig>;
template class mapHandlerWriteDispatch<24, 1, -1, endianLittle>;
template class mapHandlerWriteDispatch<24, 1, -1, endianBig>;

template class mapHandlerWriteDispatch<17, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<17, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<18, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<18, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<19, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<19, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<20, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<20, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<21, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<21, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<22, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<22, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<23, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<23, 2, 0, endianBig>;
template class mapHandlerWriteDispatch<24, 2, 0, endianLittle>;
template class mapHandlerWriteDispatch<24, 2, 0, endianBig>;

template class mapHandlerWriteDispatch<17, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<17, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<18, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<18, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<19, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<19, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<20, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<20, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<21, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<21, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<22, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<22, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<23, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<23, 2, -1, endianBig>;
template class mapHandlerWriteDispatch<24, 2, -1, endianLittle>;
template class mapHandlerWriteDispatch<24, 2, -1, endianBig>;

template class mapHandlerWriteDispatch<17, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<17, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<18, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<18, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<19, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<19, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<20, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<20, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<21, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<21, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<22, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<22, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<23, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<23, 2, -2, endianBig>;
template class mapHandlerWriteDispatch<24, 2, -2, endianLittle>;
template class mapHandlerWriteDispatch<24, 2, -2, endianBig>;

template class mapHandlerWriteDispatch<17, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<17, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<18, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<18, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<19, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<19, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<20, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<20, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<21, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<21, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<22, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<22, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<23, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<23, 3, 0, endianBig>;
template class mapHandlerWriteDispatch<24, 3, 0, endianLittle>;
template class mapHandlerWriteDispatch<24, 3, 0, endianBig>;

template class mapHandlerWriteDispatch<17, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<17, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<18, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<18, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<19, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<19, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<20, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<20, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<21, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<21, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<22, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<22, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<23, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<23, 3, -1, endianBig>;
template class mapHandlerWriteDispatch<24, 3, -1, endianLittle>;
template class mapHandlerWriteDispatch<24, 3, -1, endianBig>;

template class mapHandlerWriteDispatch<17, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<17, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<18, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<18, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<19, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<19, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<20, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<20, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<21, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<21, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<22, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<22, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<23, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<23, 3, -2, endianBig>;
template class mapHandlerWriteDispatch<24, 3, -2, endianLittle>;
template class mapHandlerWriteDispatch<24, 3, -2, endianBig>;

template class mapHandlerWriteDispatch<17, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<17, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<18, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<18, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<19, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<19, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<20, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<20, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<21, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<21, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<22, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<22, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<23, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<23, 3, -3, endianBig>;
template class mapHandlerWriteDispatch<24, 3, -3, endianLittle>;
template class mapHandlerWriteDispatch<24, 3, -3, endianBig>;
