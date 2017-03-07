
#pragma once

#include "emu/devsys.h"

class cpuDevice : public Device
{
public:
	cpuDevice();
	virtual ~cpuDevice();

	virtual void execute() = 0;
};
