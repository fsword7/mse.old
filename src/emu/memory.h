/*
 * memory.h - Memory Manager facility
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

class Device;
class memAddressMap;

using offs_t = std::size_t;

class memManager {
public:
	memManager(Device *sys);
	~memManager();

private:
};


class memAddressEntry {
public:
	memAddressEntry(Device &dev, memAddressMap &map, offs_t start, offs_t end);

	// Address entry information (public access)
	memAddressEntry *next;	// Point to the next entry
	memAddressMap   &map;	// Reference to address map database
	Device			&dev;	// Reference to base device

	// Address parameters (public address)
	offs_t		adrStart;	// Start address
	offs_t		adrEnd;		// End address
	offs_t		adrMask;	// Mask address bits
	offs_t		adrMirror;	// Mirror address bits
	offs_t		adrSelect;	// Select address bits
};

class memAddressMap {
public:
	memAddressMap(Device &sys);
	~memAddressMap();

	memAddressEntry &operator()(offs_t start, offs_t end);

private:
	Device &sys;
	std::vector<memAddressEntry> list;
};
