/*
 * devcpu2.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devcpu.h"

cpu_device::cpu_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, tag, owner, clock),
  di_execute(this),
  di_memory(this)
{

}

cpu_device::~cpu_device()
{
}
