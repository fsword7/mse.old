/*
 * machine.h - running machine (scheduler)
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class rom_loader;
class console_base;

class machine {
public:
	machine(const system_config *config);
	~machine();

	mapMemoryManager &memory() { return sysMemory; }

	device_t *getSystemDevice() { return sysDevice; }
	const device_t *getSystemsDevice() const { return sysDevice; }

	device_t *systemDevice() { return sysDevice; }
	const device_t *systemDevice() const { return sysDevice; }

	const system_config &config() const { return sysConfig; }
	const system_driver &driver() const { return sysDriver; }

	void setDeviceName(tag_t *name);
	tag_t *getDeviceName();

	void init(const console_base &cty);
	void execute();

private:

	const system_config &sysConfig;
	const system_driver &sysDriver;
	device_t *sysDevice;

	rom_loader *romLoader;

	// embedded managers
	mapMemoryManager	sysMemory;
};
