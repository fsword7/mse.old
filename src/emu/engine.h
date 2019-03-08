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
	system_engine(const cty_t &cty) : cty(cty) {}

	machine *find(std::string drvName);
	device_t *findDevice(device_t &sys, std::string drvName) const;

	void create(std::string drvName, std::string tagName);
	void list();

	// debug tools
	void dump(int argc, args_t &args);
	void dumpr(int argc, args_t &args);

private:
	static std::vector<machine> machines;
	const cty_t &cty;
};
