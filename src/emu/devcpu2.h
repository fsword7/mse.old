/*
 * devcpu2.h
 *
 *  Created on: Jan 10, 2019
 *      Author: Tim Stark
 */

#pragma once

class cpu_device : public device_t
{
protected:
	cpu_device(const char *tag, const char *drv, device_t *owner);

public:
	virtual ~cpu_device();

};
