/*
 * devcpu2.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devcpu.h"

cpu_device::cpu_device(const system_config &config, const device_type_base &type,
		tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, type, tag, owner, clock),
  di_execute(this),
  di_memory(this),
  di_debug(this)
{

}

cpu_device::~cpu_device()
{
}
