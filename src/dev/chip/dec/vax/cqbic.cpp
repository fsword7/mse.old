/*
 * cqbic.cpp - CVAX Qbus Interface Chip
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cqbic.h"

DEFINE_DEVICE_TYPE(CQBIC, cqbic_device, "CQBIC", "CVAX Qbus Interface Chip")

cqbic_device::cqbic_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CQBIC, tag, owner, clock)
{
}

cqbic_device::~cqbic_device()
{
}
