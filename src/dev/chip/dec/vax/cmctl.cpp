/*
 * cmctl.cpp - CVAX Memory Controller
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cmctl.h"

DEFINE_DEVICE_TYPE(CMCTL, cmctl_device, "CMCTL", "CVAX Memory Controller")

cmctl_device::cmctl_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CMCTL, tag, owner, clock)
{
}

cmctl_device::~cmctl_device()
{
}
