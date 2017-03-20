/*
 * ka650.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Timothy Stark
 */

#pragma once

class ka650_sysDevice : public vax_sysDevice
{
public:
	ka650_sysDevice();
	~ka650_sysDevice();

	static ka650_sysDevice *create(std::string devName, std::string devType, sysModel *model);
};
