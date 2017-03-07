
#pragma once

class cpuDevice
{
public:
	cpuDevice();
	virtual ~cpuDevice();

	virtual void execute() = 0;
};
