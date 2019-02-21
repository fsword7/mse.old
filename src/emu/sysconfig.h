/*
 * sconfig.h - System (Machine) Configuration
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

template <typename Tag, typename Creator, typename... Params>
inline device_t *cfgAddDevice(system_config &config, Tag *tag, Creator &&type, Params &&... args)
{
	return &type(config, std::forward<Tag>(tag), std::forward<Params>(args)...);
}

class system_config
{
public:
	system_config(const system_driver &driver);

	std::pair<const char *, device_t *> resolveOwner(const char *tag);

	device_t *addDevice(const char *tag);

	device_t *addDevice(device_t *dev, device_t *owner);

	template <typename Creator, typename... Params>
	auto addDevice(tag_t *tag, Creator &&type, Params &&... args)
	{
//		std::pair<tag_t *, device_t *> const owner(resolveOwner(tag));
//		auto device(type.create(*this, owner.first, owner.second, std::forward<Params>(args)...));
//		auto &result(*device);
//		addDevice(device, owner.second);
//		return &result;
	}

	device_t *getCurrentDevice() { return curDevice; }
	device_t *getCurrentDevice() const { return curDevice; }

private:
	const system_driver	&sysDriver;		// system driver
	device_t			*sysDevice;		// root system device
	device_t			*curDevice;		// current device
};
