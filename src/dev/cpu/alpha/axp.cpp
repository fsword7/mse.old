/*
 * axp.cpp
 *
 *  Created on: Mar 7, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/alpha/axp.h"

axp_cpu_base::axp_cpu_base(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: cpu_device(config, tag, owner, clock)
{

}

axp_cpu_base::~axp_cpu_base()
{
}

