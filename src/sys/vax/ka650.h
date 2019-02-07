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
	ka650_device(const char *tag, const system_config &config, uint64_t clock)
	: system_device(tag, config, nullptr, clock)
	{
	}

	// System creator routines
	void mv3900();
	void mv3900x();

	// Address space mapping
	void mv3900_mem(mapAddress &map);
	void mv3900x_mem(mapAddress &map);

	// Model-specific system initialize routines
	static void mv3900_init();
	static void mv3900x_init();
};
