/*
 * devcpu2.h
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#pragma once

class cpu_device : public device_t
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
	cpu_device(const char *tag, const system_config &config, device_t *owner, uint64_t clock);

public:
	virtual ~cpu_device();

protected:
	cpuState state;
};
