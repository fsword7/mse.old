/*
 * devsys.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#pragma once

class Device;
class Console;

// Return code (error codes)
#define CMD_OK         0
#define CMD_SHUTDOWN   1
#define CMD_NOTFOUND   2
#define CMD_ERROR      3

typedef std::vector<std::string> args_t;

typedef int (*cmdfunc)(Console *, Device *, args_t &args);

struct Command {
	const char *name;
	const char *usage;
	cmdfunc execute;
};

struct Driver
{
	// Drive type information
	const char *drvName;  // Driver name
	const char *drvDesc;  // Driver description
	const char *srcFile;  // Source file
	const char *pdevName; // Parent driver name

	// Command handler
	const Command *Commands;
	const Command *setCommands;
	const Command *showCommands;
};

class Device
{
public:
	Device();
	virtual ~Device();

	inline Driver *getDriver() const { return driver; }

	Device *findDevice(std::string devName);
	Driver *findDriver(std::string drvName);

	void setDriver(Driver *driver) { this->driver = driver; }
	void setParent(Device *parent) { this->parent = parent; }

	void setDrivers(Driver **drivers) { this->drivers = drivers; }

	// Device virtual function calls
	virtual int load(std::string fname);

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
