/*
 * block.h - memory block manager
 *
 *  Created on: Mar 25, 2019
 *      Author: Tim Stark
 */

#pragma once

class mapMemoryBlock
{
public:
	mapMemoryBlock(mapAddressSpace &space, offs_t start, offs_t end, uint8_t *memory = nullptr);
	~mapMemoryBlock();

	// Getters
	machine &getMachine() const { return system; }
	offs_t getAddrStart() const	{ return addrStart; }
	offs_t getAddrEnd() const	{ return addrEnd; }
	uint8_t *base() const		{ return data; }
	bool isAllocated() const	{ return flagAllocated; }

private:
	machine				&system;
	mapAddressSpace		&space;

	// Start/end addresses
	offs_t	addrStart;
	offs_t	addrEnd;
	bool	flagAllocated;

	uint8_t *data;

	// memory data area
	std::vector<uint8_t> allocated;
};
