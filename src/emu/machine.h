/*
 * machine.h - running machine (scheduler)
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class machine {
public:
	machine(const system_config &config);
	~machine();

	device_t *sysDevice() { return device; }
	const device_t *sysDevice() const { return device; }

	void init();
	void execute();

private:

	const system_config &config;
	device_t *device;

	// embedded managers
	mapManager		memory;
};
