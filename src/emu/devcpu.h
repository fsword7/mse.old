
#pragma once

class cpu_Device
{
public:
	cpu_Device();
	virtual ~cpu_Device();

	virtual void execute() = 0;
};
