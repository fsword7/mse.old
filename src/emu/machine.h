/*
 * machine.h - running machine (scheduler)
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class machine {
public:
	machine(const system_config *config);
	~machine();

	device_t *getSystemDevice() { return sysDevice; }
	const device_t *getSystemsDevice() const { return sysDevice; }

	void init();
	void execute();

private:

	const system_config &config;
	device_t *sysDevice;

	// embedded managers
	mapManager		memory;
};
