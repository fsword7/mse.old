/*
 * engine.h
 *
 *  Created on: Jan 13, 2019
 *      Author: Tim Stark
 */

#pragma once

class system_engine
{
public:
	system_engine() {}

//	static device_t *find(std::string drvName);

	static void create(std::string drvName, std::string tagName);

private:
	static std::vector<machine> machines;
};
