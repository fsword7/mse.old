/*
 * devcpu2.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devcpu.h"

cpu_device::cpu_device(const char *tag, const system_config &config, device_t *owner, uint64_t clock)
: device_t(tag, config, owner, clock)
{

}

cpu_device::~cpu_device()
{
}
