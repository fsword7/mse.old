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

	void start();
	void execute();

private:

	const system_config &config;

	// embedded managers
	mapManager		memory;
};
