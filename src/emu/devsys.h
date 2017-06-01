/*
 * devsys.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Timothy Stark
 */

#pragma once

class logFile;
class Device;
class sysDevice;
class cpuDevice;
class Console;
struct sysModel;
struct cfgMemory;

// Return code (error codes)
#define CMD_OK         0
#define CMD_SHUTDOWN   1
#define CMD_NOTFOUND   2
#define CMD_ERROR      3

// Device Class Type
#define CLS_UNKNOWN    0
#define CLS_ROOT       1
#define CLS_SYSTEM     2
#define CLS_CPU        3
#define CLS_IODEV      4

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

	// Configurations
	cfgMemory  *cfgMemory;

	// Command handler
	const Command *Commands;
	const Command *setCommands;
	const Command *showCommands;
	const Command *listCommands;

	// Function calls
	Device *(*create)(std::string, std::string, sysModel *);
};

struct sysModel {
	const char *name;     // System/model name
	const char *parent;   // Parent system
	const char *desc;     // Description
	Driver     *driver;   // System driver for configurations
	const char *srcFile;  // Source file
};

struct cfgMemory {
	const char *cfgName;
	uint64_t	cfgSize;
};


#define SYSTEM(name, desc, driver)         { #name, nullptr, desc, driver, __FILE__ }
#define MODEL(name, parent, desc, driver)  { #name, #parent, desc, driver, __FILE__ }

class Device
{
public:
	Device(int clsType);
	virtual ~Device();

	inline std::string getName() const        { return devName; }
	inline std::string getType() const        { return devType; }
	inline std::string getDescription() const { return devDesc; }
	inline Driver *getDriver() const          { return driver; }
	inline int getClass() const               { return clsType; }
#ifdef ENABLE_DEBUG
	inline Debug *getDebug()				  { return &dbg; }
#endif /* ENABLE_DEBUG */

	// Function calls for driver table
	cfgMemory *getMemoryConfig() const;
	const Command   *getCommandTable() const;
	const Command   *getSetCommandTable() const;
	const Command   *getShowCommandTable() const;
	const Command   *getListCommandTable() const;

	Device *findDevice(std::string devName);
	Driver *findDriver(std::string drvName);
	sysModel *findModel(std::string sysName);

	void setDriver(Driver *driver)     { this->driver = driver; }
	void setParent(Device *parent)     { this->parent = parent; }
	void setSystemDevice(sysDevice *sdev);

	void setDrivers(Driver **drivers) { this->drivers = drivers; }
	void setModels(sysModel **models) { sysModels = models; }

	void addDevice(Device *dev)       { devices.push_back(dev); }

	// Device virtual function calls
	virtual int setMemory(uint32_t size);
	virtual int boot();
	virtual int load(std::string fname, uint32_t sAddr);
	virtual int dump(uint32_t *Addr, uint32_t eAddr, uint32_t sw);

protected:
	std::string devName;  // Device name
	std::string devType; // Device type (model)
	std::string devDesc;  // Device description

	int      clsType;
	Driver  *driver; // Driver table information
	Device  *parent;  // parent device
	std::vector<Device*> devices; // child device table
	Driver  **drivers; // child driver table
	sysModel **sysModels;

	sysDevice *sdev;
#ifdef ENABLE_DEBUG
	Debug      dbg;
#endif /* ENABLE_DEBUG */
};

class sysDevice : public Device
{
public:
	sysDevice();
	~sysDevice();

	void addCPUDevice(cpuDevice *cdev);

	cpuDevice *getCPUDevice(int idx);

	inline logFile *getLogFile()  { return &log; }

	virtual uint32_t readio(uint32_t pAddr, uint32_t acc);
	virtual void     writeio(uint32_t pAddr, uint32_t data, uint32_t acc);

protected:
	std::vector<cpuDevice *> cpu;
	logFile log;
};

void setSystemDrivers(Device *dev);
