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


private:
	static system_driver const *syslist[];
};
