/*
 * device.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/device.h"

device_t::device_t(const char *tag)
: devOwner(nullptr), devNext(nullptr),
  tagName(tag), drvName(nullptr)
{
}

device_t::~device_t()
{
}



