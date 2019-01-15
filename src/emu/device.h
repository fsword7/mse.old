/*
 * device.h - Base Device Class facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <typeinfo>

class system_config;
class device_t;

template<class SystemClass>
struct system_tag_struct { typedef SystemClass type; };

template<class SystemClass>
auto system_tag_func() { return system_tag_struct<SystemClass>{ }; };

class device_type_base
{
private:
	typedef device_t *(*create_func)(const char *tag, device_type_base const &type, system_config &config, device_t *owner);

	template<typename SystemClass>
	static device_t *create_system(const char *tag, device_type_base const &type, system_config &config, device_t *owner)
	{
		return new SystemClass(tag, type, config);
	}

public:
	template <class SystemClass>
	device_type_base(system_tag_struct<SystemClass>(*)())
	: typeInfo(typeid(SystemClass)),
	  creator(&create_system<SystemClass>)
	{
	}

private:

	const std::type_info &typeInfo;
	const create_func     creator;
};

//template <typename DeviceClass, const char *name>
//constexpr auto device_creator = &device_tag_func<DeviceClass, name>;

template <typename SystemClass>
constexpr auto system_creator = &system_tag_func<SystemClass>;

typedef device_type_base const &device_type;

class device_t : public delegate_bind
{
private:
	class device_list
	{
	public:
		device_list() {}

	private:
		std::vector<device_list> list;
	};

	class interface_list
	{

	};

protected:
	device_t(const char *tag, device_type type, const system_config &config, device_t *owner);

public:
	virtual ~device_t();

	const char *tag() { return tagName.c_str(); }
//	const char *name() { return drvName.c_str(); }
	std::string &name() { return drvName; }
	device_t *owner() { return devOwner; }
	device_t *next()  { return devNext; }

protected:
	device_type		typeInfo;
	device_t 		*devOwner;			// Parent device owner
	device_t 		*devNext;			// Next device of the same owner
	device_list 	deviceList;			// List of child devices - container
	interface_list	interfaceList;		// List of interfaces - container

	const system_config &sysConfig;

private:
	std::string		tagName;			// Tag name for linking named devices
	std::string		drvName;			// Device name for command line access

};
