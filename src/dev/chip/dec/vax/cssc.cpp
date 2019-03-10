/*
 * cssc.cpp - CVAX System Support Chip
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cssc.h"

DEFINE_DEVICE_TYPE(CSSC, cssc_device, "CSSC", "CVAX System Support Chip")

cssc_device::cssc_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CSSC, tag, owner, clock)
{
}

cssc_device::~cssc_device()
{
}

