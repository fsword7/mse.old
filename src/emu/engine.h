/*
 * engine.h
 *
 *  Created on: Jan 13, 2019
 *      Author: fswor
 */

#pragma once

class system_engine
{
public:
	system_engine() {}

	static device_t *find(std::string drvName);

	static device_t *create(std::string drvName, std::string tagName);

private:
	static std::vector<device_t> machine;
};
