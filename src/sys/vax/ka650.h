/*
 * ka650new.h
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#pragma once

class ka650_device : public system_device
{
public:
	ka650_device(const char *tag, const system_config &config, uint64_t clock)
	: system_device(tag, config, nullptr, clock)
	{
	}

	// System creator routines
	static void mv3900();
	static void mv3900x();

	// Model-specific system initialize routines
	static void mv3900_init();
	static void mv3900x_init();
};
