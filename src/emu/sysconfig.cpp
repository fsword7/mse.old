/*
 * sysconfig.cpp
 *
 *  Created on: Jan 14, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/devsys.h"
#include "emu/sysconfig.h"


system_config::system_config(const system_driver &driver)
: sysDriver(driver), sysDevice(nullptr), cfgDevice(nullptr)
{
	// Create root of system device
	addDeviceType(driver.name, driver.type, 0);

}

device_t *system_config::addDeviceType(tag_t *tag, const device_type_base &type, uint64_t clock)
{
	std::pair<tag_t *, device_t *> const owner = resolveOwner(tag);

	std::cout << "Creating " << type.sname() << ": tag " << tag << " -> " << owner.first << std::endl;

	device_t *dev = type.create(*this, owner.first, owner.second, clock);
	return addDevice(dev, owner.second);
}

device_t *system_config::addDevice(device_t *dev, device_t *owner)
{
	// Save the current configuring device and will automatically restore it
	// after new device configuration process and exiting this function call.
	const config_device_stack context(*this);

	if (owner != nullptr) {
		owner->devices().add(dev);
	} else {
		assert(sysDevice == nullptr);
		dynamic_cast<system_device *>(dev)->setSystemDriver(&sysDriver);
		sysDevice = dev;
	}

	// device configuration process
	dev->configure(*this);

	return dev;
}

void system_config::begin(device_t *device)
{
	// Assign device/system for configuration process
	assert(cfgDevice == nullptr);
	cfgDevice = device;
}

std::pair<const char *, device_t *> system_config::resolveOwner(const char *tag)
{
	const char	*otag = tag;
	device_t	*owner = cfgDevice;
	std::string	name;

	// Determine absolute path name
	if (tag[0] == ':') {
		cfgDevice = sysDevice;
		tag++;
	}

	while (strchr(tag, ':')) {
		const char *next = strchr(tag, ':');
		name.assign(tag, next - tag);
		owner = owner->devices().find(name);
		tag = next + 1;
	}
	assert(tag[0] != '\0');

	return std::make_pair(tag, owner);
}
