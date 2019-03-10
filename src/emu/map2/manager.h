/*
 * manager.h - Memory Manager for system machines
 *
 *  Created on: Mar 10, 2019
 *      Author: Tim Stark
 */

#pragma once

class mapMemoryManager
{
public:
	mapMemoryManager();
	~mapMemoryManager();

private:
	machine &system;
};
