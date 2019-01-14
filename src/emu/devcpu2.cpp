/*
 * devcpu2.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devcpu2.h"

cpu_device::cpu_device(const char *tag, device_type type, const system_config &config, device_t *owner)
: device_t(tag, type, config, owner)
{

}

cpu_device::~cpu_device()
{
}
