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
#include "emu/machine.h"
#include "emu/validity.h"

device_t::device_t(const system_config &config, const device_type_base &type, tag_t *tag, device_t *owner, uint64_t clock)
: type(type), devOwner(owner), devNext(nullptr),
  tagName(tag), devName(""),
  sysConfig(config),
  system(nullptr)
{
	acList.clear();
}

device_t::~device_t()
{
}

void device_t::beginConfig(system_config *config)
{
	assert(&sysConfig == config);
	config->beginConfig(this);
	processConfig(*config);

//	for(devauto_base *autodev : acList)
//		autodev->endConfig();
}

devauto_base *device_t::register_device(devauto_base *autodev)
{
	devauto_base *last = nullptr;

	// add device to auto device list
	if (acList.size() > 0)
		last = acList.back();
	acList.push_back(autodev);

	return last;
}

void device_t::resolvePreMap()
{
}

void device_t::resolvePostMap()
{
}

void device_t::validate(validity_checker &valid) const
{
	for (device_interface *intf : interfaces())
		intf->validate(valid);

	validateDevice(valid);
}

void device_t::processConfig(system_config &config)
{
	// Do nothing by default
}

void device_t::validateDevice(validity_checker &valid) const
{
	// Do nothing by default
}

// ********************************************************

device_interface::device_interface(device_t *dev, tag_t *name)
: next(nullptr), device(dev), typeName(name)
{
	device_t::interface_list &list = dev->interfaces();

	// Add interface into device's interface list.
	if (list.size() > 0)
		list.last()->next = this;
	list.add(this);
}

device_interface::~device_interface()
{
}

void device_interface::validate(validity_checker &valid) const
{
	// Do nothing by default
}
