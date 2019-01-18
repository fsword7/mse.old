/*
 * device.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/device.h"

device_t::device_t(const char *tag, device_type type, const system_config &config, device_t *owner)
: typeInfo(type), devOwner(owner), devNext(nullptr),
  tagName(tag), drvName(nullptr),
  sysConfig(config)
{
}

device_t::~device_t()
{
}



