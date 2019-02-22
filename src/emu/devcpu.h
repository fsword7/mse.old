/*
 * devcpu2.h
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"
#include "emu/diexec.h"
#include "emu/dimem.h"
#include "emu/didebug.h"

class cpu_device
: public device_t,
  public di_execute,
  public di_memory,
  public di_debug
{
public:
	enum cpuSignal {
		cpuStop,
		cpuStart,
		cpuInterrupt
	};

	enum cpuState {
		cpuStopped = 0,
		cpuStopping,
		cpuRunning,
		cpuIdle,
		cpuWait
	};

protected:
	cpu_device(const system_config &config, const device_type_base &type,
			tag_t *tag, device_t *owner, uint64_t clock);

public:
	virtual ~cpu_device();

protected:
	cpuState state;
};
