/*
 * device.h - Base Device Class facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once



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

public:
	virtual ~device_t();

	const char *tag() { return tagName.c_str(); }
	device_t *owner() { return devOwner; }
	device_t *next()  { return devNext; }

protected:

	device_t 		*devOwner;			// Parent device owner
	device_t 		*devNext;			// Next device of the same owner
	device_list 	deviceList;			// List of child devices - container
	interface_list	interfaceList;		// List of interfaces - container

private:
	std::string		tagName;			// Tag name for linking named devices
	std::string		devName;			// Device name for command line access

};
