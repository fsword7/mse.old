/*
 * ev4.h
 *
 *  Created on: Mar 28, 2016
 *      Author: Tim Stark
 */

#pragma once

class ev4_cpuDevice : public cpu_Device
{
public:
	ev4_cpuDevice();
	~ev4_cpuDevice();

	void execute();
};
