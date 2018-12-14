
#pragma once

#include "emu/devsys.h"

class cpuDevice : public Device
{
public:
	cpuDevice();
	virtual ~cpuDevice();

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

	virtual void reset() = 0;
	virtual void execute() = 0;
	virtual int  boot();
	virtual int  start();
	virtual int  stop();

	virtual void send(cpuSignal signal);

protected:
	cpuState state;

private:
};
