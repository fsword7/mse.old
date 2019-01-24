/*
 * device.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/device.h"

device_t::device_t(const char *tag, const system_config &config, device_t *owner, uint64_t clock)
: devOwner(owner), devNext(nullptr),
  tagName(tag), drvName(""),
  sysConfig(config)
{
}

device_t::~device_t()
{
}



