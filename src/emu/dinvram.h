/*
 * dinvram.h - NVRAM device interface
 *
 *  Created on: Mar 21, 2019
 *      Author: Tim Stark
 */

#pragma once

class di_nvram : public device_interface
{
public:
	di_nvram(system_config &config, device_t *device);
	virtual ~di_nvram();

	void reset() { nvramDefault(); }
	void load(emuFile &file) { nvramRead(file); }
	void save(emuFile &file) { nvramWrite(file); }

protected:
	virtual void nvramDefault() = 0;
	virtual void nvramRead(emuFile &file) = 0;
	virtual void nvramWrite(emuFile &file) = 0;
};

typedef device_interface_iterator<di_nvram> nvram_interface_iterator;
