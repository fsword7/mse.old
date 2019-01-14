/*
 * sconfig.h - System (Machine) Configuration
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

class system_config
{
public:
	system_config(const system_driver &driver);

	device_t *addDevice(const char *tag, device_type type);

private:
	const system_driver	&sysDriver;
	device_t			*sysDevice;
};
