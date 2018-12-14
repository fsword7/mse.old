#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"
#include "emu/devcpu.h"

cpuDevice::cpuDevice()
: Device(CLS_CPU)
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
