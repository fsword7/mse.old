/*
 * dinvram.cpp - NVRAM device interface
 *
 *  Created on: Mar 21, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/emufile.h"
#include "emu/dinvram.h"

di_nvram::di_nvram(system_config &config, device_t *device)
: device_interface(device, "nvram")
{
}

di_nvram::~di_nvram()
{
}

