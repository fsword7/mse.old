/*
 * devsys2.cpp - system driver device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys.h"

system_device::system_device(const system_config &config, const device_type_base &type,
		tag_t *tag, uint64_t clock)
: device_t(config, type, tag, nullptr, clock)
{

}

system_device::~system_device()
{
}

// init - default implementation does nothing
void system_device::init()
{
}

