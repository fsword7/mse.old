
#include "../old/devcpu-old.h"

#include "../old/devsys-old.h"
#include "emu/core.h"
#include "emu/debug.h"

cpuDevice::cpuDevice()
: Device(CLS_CPU),
  state(cpuStopped)
{
}

cpuDevice::~cpuDevice()
{
}

int cpuDevice::boot()
{
	// Reset CPU device for cold boot
	reset();

	// Create thread process and start execute routine.
	process = std::thread([=]{ execute(); });

	return CMD_OK;
}

int cpuDevice::start()
{
	return CMD_OK;
}

int cpuDevice::stop()
{
	// Send STOP signal to processor and wait for
	// thread process to be terminated
	send(cpuStop);
	process.join();

	return CMD_OK;
}

void cpuDevice::send(cpuSignal)
{
}
