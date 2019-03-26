/*
 * addrmap.h - Address mapping entries
 *
 *  Created on: Mar 10, 2019
 *      Author: fswor
 */

#pragma once

enum mapAddressType {
	mapNone = 0,
	mapRAM,			// RAM type
	mapROM,			// ROM type
	mapPort,		// I/O port type
	mapNop,			// Non-existent type
	mapUnmapped,	// Unmapped type
	mapBank,		// Memory bank type
	mapDelegate,	// Device delegate (callback)
	mapSubmap		// Device submap type
};

struct mapHandler
{
	mapAddressType type = mapNone;
	uint8_t bits = 0;
	tag_t *tag = nullptr;
	tag_t *name = nullptr;
};

class mapAddressEntry
{
};

class mapAddress
{
private:
	std::vector <mapAddressEntry> list;
};
