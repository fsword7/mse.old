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

	// Getters
	std::vector<mapMemoryBlock *> &blocks() const		{ return blockList; }
	std::vector<mapMemoryBank *> &banks() const			{ return bankList; }
	std::vector<mapMemoryRegion *> &regions() const		{ return regionList; }
	std::vector<mapMemoryShare *> &shares() const		{ return shareList; }

private:
	machine &system;

	// Memory spaces
	std::vector<mapMemoryBlock *> blockList;
	std::vector<mapMemoryBank *> bankList;
	std::vector<mapMemoryRegion *> regionList;
	std::vector<mapMemoryShare *> shareList;
};
