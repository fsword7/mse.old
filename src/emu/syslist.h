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


	const system_driver *find(std::string name);
//	const system_driver &find(std::string name);
	const system_driver *clone(const system_driver *driver);

	void list();

private:
	static system_driver const *syslist[];
};
