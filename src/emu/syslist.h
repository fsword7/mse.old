/*
 * syslist.h
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#pragma once


class system_list
{
public:
	system_list() {}


	static const system_driver *find(std::string name);

	static void list();

private:
	static system_driver const *syslist[];
};
