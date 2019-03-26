/*
 * space.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Tim Stark
 */

#pragma once

enum class mapSpaceType {
	spaceProgram	= 0,
	spaceData		= 1,
	spaceIO			= 2
};

enum class rwType {
	READ	= 2,
	WRITE	= 1,
	RW		= (READ|WRITE)
};

class mapAddressSpaceConfig
{
public:
	mapAddressSpaceConfig(tag_t *name, uint8_t dataWidth, uint8_t dataRadix,
		uint8_t addrWidth, uint8_t addrRadix, uint8_t addrShift = 0);

	~mapAddressSpaceConfig();

	// Getters
	tag_t   *getTagName() const { return tagName; }
	endian_t getEndian() const { return endian; }
	uint8_t  getAddressWidth() const { return addrWidth; }
	uint8_t  getAddressRadix() const { return addrRadix; }
	uint8_t  getAddressShift() const { return addrShift; }
	uint8_t  getDataWidth() const { return dataWidth; }
	uint8_t  getDataRadix() const { return dataRadix; }

private:
	tag_t		*tagName;	// Address space name
	endian_t 	endian;		// Endian type (liitle or big)
	uint8_t		dataWidth;	// Data width
	uint8_t 	dataRadix;	// Data raidx
	uint8_t		addrWidth;	// Address width
	uint8_t		addrRadix;	// Address radix
	uint8_t		addrShift;	// Address shift
};

class mapAddressSpace
{
public:
	mapAddressSpace(mapMemeoryManager &manger, mapSpaceType space);
	~mapAddressSpace();

	// Getters
	tag_t   *getTagName() const { return tagName; }
	endian_t getEndian() const { return endian; }
	uint8_t  getAddressWidth() const { return addrWidth; }
	uint8_t  getAddressRadix() const { return addrRadix; }
	uint8_t  getAddressShift() const { return addrShift; }
	uint8_t  getDataWidth() const { return dataWidth; }
	uint8_t  getDataRadix() const { return dataRadix; }

	// Memory space initialization routines
	void prepare(const cty_t &cty);

private:
	mapAddressSpaceConfig	&config;
	mapMemoryManager		&manager;
	mapSpaceType			space;

	tag_t		*tagName;	// Address space name
	endian_t 	endian;		// Endian type (liitle or big)
	uint8_t		dataWidth;	// Data width
	uint8_t 	dataRadix;	// Data raidx
	uint8_t		addrWidth;	// Address width
	uint8_t		addrRadix;	// Address radix
	uint8_t		addrShift;	// Address shift
};
