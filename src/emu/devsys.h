/*
 * devsys.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#pragma once

struct Driver
{
	const char *devName;  // Device name
	const char *devDesc;  // Device description
	const char *srcFile;  // Source file
	const char *pdevName; // Parent device name

};

class Device
{
public:
	Device();
	~Device();

	inline Driver *getDriver() const { return driver; }

	Device *findDevice(std::string devName);
	Driver *findDriver(std::string drvName);

	void setDriver(Driver *driver) { this->driver = driver; }
	void setParent(Device *parent) { this->parent = parent; }

	void setDrivers(Driver **drivers) { this->drivers = drivers; }

protected:
	Driver  *driver; // Driver table information
	Device  *parent;  // parent device
	std::vector<Device*> devices; // child device table
	Driver  **drivers; // child driver table
};

class sysDevice : public Device
{
public:
	sysDevice();
	~sysDevice();

private:
};

void setSystemDrivers(Device *dev);

//struct sysDriver
//{
//	const char *srcFile;
//	const char *name;
//	const char *parent;
//	const char *desc;
//	const char *manfacturer;
//	const char *year;
//};


#if 0
#define SYS_NAME(name) sysDriver_##name

#define SYSTEM(name, parent, fname) \
extern const sysDriver SYS_NAME(name) = \
{              \
	__FILE__,  \
	#name,     \
	#parent,   \
	fname,     \
	nullptr,   \
	nullptr    \
};

#define GROUP(name) \
extern const sysDriver SYS_NAME(name) = \
{              \
	__FILE__,  \
	#name,     \
	nullptr,   \
	nullptr,   \
	nullptr,   \
	nullptr    \
};
#endif
