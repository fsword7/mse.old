/*
 * device.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "emu/devauto.h"

device_t::device_t(const char *tag, const system_config &config, device_t *owner, uint64_t clock)
: devOwner(owner), devNext(nullptr),
  tagName(tag), devName(""),
  sysConfig(config),
  autodevList(nullptr)
{
}

device_t::~device_t()
{
}

devauto_base *device_t::register_device(devauto_base *autodev)
{
	devauto_base *old;

	// add device to auto device list
	old = autodevList;
	autodevList = autodev;

	return old;
}


// ********************************************************

device_interface::device_interface(device_t &dev, tag_t *name)
: next(nullptr), device(dev), typeName(name)
{

}

device_interface::~device_interface()
{
}
