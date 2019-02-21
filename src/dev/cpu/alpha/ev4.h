/*
 * ev4.h
 *
 *  Created on: Mar 28, 2016
 *      Author: Tim Stark
 */

#pragma once

#include "dev/cpu/alpha/axp.h"

class ev4_cpu : public axp_cpu_base
{
public:
	ev4_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);
	~ev4_cpu();

	void execute();
};
