/*
 * device.h - Base Device Class facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <typeinfo>

class system_config;
class device_list;
class device_t;
class devauto_base;

class device_t : public delegate_bind
{
	friend class devauto_base;

private:
	class device_list
	{
	public:
		device_list() {}

		device_t *find(const std::string &name) const
		{
			for(auto cdev : list) {
				if (cdev->tag() == name)
					return cdev;
			}
			return nullptr;
		}

		void add(device_t *dev) {
			list.push_back(dev);
		}

	private:
		std::vector<device_t *> list;
	};

	class interface_list
	{
	public:
		interface_list() {}

	private:
		interface_list	*head;
//		di_execute		*execute;
//		di_memory 		*memory;
	};

protected:
	device_t(const char *tag, const system_config &config, device_t *owner, uint64_t clock);

public:
	virtual ~device_t();

	template <class DeviceClass>
	static device_t *create(const char *tag, const system_config &config, device_t *owner, uint64_t clock)
	{
		return new DeviceClass(tag, config, clock);
	}

//	const char *tag() { return tagName.c_str(); }
	std::string &tag() { return tagName; }
//	const char *name() { return drvName.c_str(); }
	std::string &name() { return drvName; }
	device_t *owner() { return devOwner; }
	device_t *next()  { return devNext; }

//	void setSystemConfig(const system_config &config) { sysConfig = config; }

	device_list &devices() { return deviceList; }
	const device_list &devices() const { return deviceList; }

	interface_list &interfaces() { return interfaceList; }

protected:
	devauto_base *register_device(devauto_base *autodev);

protected:
	device_t 		*devOwner;			// Parent device owner
	device_t 		*devNext;			// Next device of the same owner
	device_list 	deviceList;			// List of child devices - container
	interface_list	interfaceList;		// List of interfaces - container

	const system_config &sysConfig;

private:
	std::string		tagName;			// Tag name for linking named devices
	std::string		drvName;			// Device name for command line access

	devauto_base	*autodevList;		// List of auto device configurations
};

class device_interface
{
protected:
	device_interface(device_t &dev, tag_t *name);
	virtual ~device_interface();

public:
	tag_t *type() { return typeName; }

protected:
	device_interface	*next;
	device_t			&device;
	tag_t				*typeName;
};
