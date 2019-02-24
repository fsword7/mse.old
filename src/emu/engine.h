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

	void create(std::string drvName, std::string tagName);
	void list();

private:
	static std::vector<machine> machines;
	const cty_t &cty;
};
