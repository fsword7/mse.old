/*
 * devsys2.cpp - system driver device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys2.h"

system_device::system_device(const char *tag, device_type type, const system_config &config, device_t *owner)
: device_t(tag, type, config, owner)
{

}

system_device::~system_device()
{
}

// init - default implementation does nothing
void system_device::init()
{
}

