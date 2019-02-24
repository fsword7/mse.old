/*
 * machine.h - running machine (scheduler)
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class rom_loader;

class machine {
public:
	machine(const system_config *config);
	~machine();

	device_t *getSystemDevice() { return sysDevice; }
	const device_t *getSystemsDevice() const { return sysDevice; }

	device_t *systemDevice() { return sysDevice; }
	const device_t *systemDevice() const { return sysDevice; }

	void setDeviceName(tag_t *name);
	tag_t *getDeviceName();

	void init();
	void execute();

private:

	const system_config &config;
	device_t *sysDevice;

	rom_loader *romLoader;

	// embedded managers
	mapManager		memory;
};
