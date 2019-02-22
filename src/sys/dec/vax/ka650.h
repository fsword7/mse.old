/*
 * ka650new.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "dev/cpu/vax/cvax.h"

class ka650_device : public system_device
{
public:
	ka650_device(const system_config &config, const device_type_base &type, tag_t *tag, uint64_t clock)
	: system_device(config, type, tag, clock)
	{
	}

	// System creator routines
	void mv3900(system_config &config);
	void mv3900x(system_config &config);

	// Address space mapping
	void mv3900_mem(mapAddress &map);
	void mv3900x_mem(mapAddress &map);

	// Model-specific system initialize routines
	static void mv3900_init();
	static void mv3900x_init();
};

