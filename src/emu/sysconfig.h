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
protected:
	class config_device_stack {
	public:
		config_device_stack(system_config &config)
		: host(config), device(config.cfgDevice)
		{
			host.cfgDevice = nullptr;
		}

		~config_device_stack()
		{
			host.cfgDevice = device;
		}

	private:
		system_config &host;
		device_t *device;
	};

public:

	system_config(const system_driver &driver);

	std::pair<const char *, device_t *> resolveOwner(const char *tag);

	device_t *addDeviceType(tag_t *tag, const device_type_base &type, uint64_t clock);

	device_t *addDevice(device_t *dev, device_t *owner);

	template <typename Creator, typename... Params>
	auto addDeviceType(tag_t *tag, Creator &&type, Params &&... args)
	{
		std::pair<tag_t *, device_t *> const owner(resolveOwner(tag));

		std::cout << "Creating " << type.sname() << ": tag " << tag << " -> " << owner.first << std::endl;

		auto device = type.create(*this, owner.first, owner.second, std::forward<Params>(args)...);
		return addDevice(device, owner.second);
	}

	const system_driver &driver() const { return sysDriver; }

	device_t *configDevice() { return cfgDevice; }
	device_t *configDevice() const { return cfgDevice; }
	device_t *systemDevice() { return sysDevice; }
	device_t *systemDevice() const { return sysDevice; }

	void begin(device_t *device);

private:
	const system_driver	&sysDriver;		// system driver
	device_t			*sysDevice;		// root system device
	device_t			*cfgDevice;		// current device
};
