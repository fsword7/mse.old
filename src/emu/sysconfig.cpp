/*
 * sysconfig.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"


system_config::system_config(const system_driver &model)
: sysDriver(model), curDevice(nullptr)
{
	// Create root of system device
	sysDevice = model.creator(model.name, *this, nullptr, 0);

	model.create(*sysDevice);

}

device_t *system_config::addDevice(const char *tag)
{
//	std::pair<const char *, device_t *> const owner = resolveOwner(tag);
//	device_t *dev = type.create(owner.first, *this, owner.second);
//
//	return addDevice(dev, owner.second);
	return nullptr;
}

device_t *system_config::addDevice(device_t *dev, device_t *owner)
{
	if (owner != nullptr) {
		owner->devices().add(dev);
		dev->addSystemConfig(*this);
	} else {
		assert(sysDevice == nullptr);
		sysDevice = dev;
		sysDevice->addSystemConfig(*this);
	}

	return dev;
}

std::pair<const char *, device_t *> system_config::resolveOwner(const char *tag)
{
	const char	*otag = tag;
	device_t	*owner = curDevice;
	std::string	name;

	// Determine absolute path name
	if (tag[0] == ':') {
		curDevice = sysDevice;
		tag++;
	}

	while (strchr(tag, ':')) {
		const char *next = strchr(tag, ':');
		name.assign(tag, next - tag);
		owner = owner->devices().find(name);
		tag = next + 1;
	}

	return std::make_pair(tag, owner);
}
