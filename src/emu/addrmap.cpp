/*
 * addrmap.cpp
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/dimem.h"
#include "emu/addrmap.h"
#include "emu/exception.h"


mapAddressEntry::mapAddressEntry(device_t &_dev, mapAddress &_map, offs_t start, offs_t end)
: mapNext(nullptr), map(_map), device(_dev),
  adrStart(start), adrEnd(end),
  adrMask(0), adrMirror(0), adrSelect(0),
  tagShare(nullptr), submapDevice(nullptr)
{

}

mapAddressEntry &mapAddressEntry::mask(offs_t mask)
{
	adrMask = mask;
	if (map.gmask != 0)
		adrMask &= map.gmask;
	return *this;
}

mapAddress::mapAddress(device_t &_dev, int space)
: device(_dev),
  adrSpace(space),
  gmask(0), unmapValue(0)
{
	di_memory			*memory;
	const mapAddressConfig	*config;

	if (!device.hasInterface(memory))
		throw mseFatalError("No memory interface for device '%s'\n",
			device.tag());

	config = memory->getAddressSpaceConfig(adrSpace);
	if (config == nullptr)
		throw mseFatalError("No memory address space configuration for device '%s'\n",
			device.tag());


	if (!memory->getAddressMap(adrSpace).isnull()) {
//		std::cout << "Trying call to delelgate 1..." << std::endl;
		memory->getAddressMap(adrSpace)(*this);
	}

	if (!config->internalMap.isnull()) {
//		std::cout << "Trying call to delelgate 2..." << std::endl;
		config->internalMap(*this);
	}

//	std::cout << "Address Map initialized." << std::endl;
//	std::cout << "Address map entries: " << list.size() << std::endl;
}

mapAddress::mapAddress(device_t &_dev, mapAddressEntry *entry)
: device(_dev),
  adrSpace(AS_PROGRAM),
  gmask(0), unmapValue(0)
{
	entry->submapDelegate.bind(_dev);
	entry->submapDelegate(*this);
}


mapAddress::~mapAddress()
{
}

mapAddressEntry &mapAddress::operator()(offs_t start, offs_t end)
{
	mapAddressEntry *entry;

	entry = new mapAddressEntry(device, *this, start, end);
	list.push_back(entry);

	return *entry;
}

void mapAddress::validate(int space) const
{
}

void mapAddress::importSubmaps(machine &sysMachine, device_t &owner, int dataWidth, endian_t endian)
{

}



mapAddressEntry &mapAddressEntry::r(read8_delegate func)
{
	assert(!func.isnull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.name();
	read8 = func;

	return *this;
}


mapAddressEntry &mapAddressEntry::r(read16_delegate func)
{
	assert(!func.isnull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.name();
	read16 = func;

	return *this;
}

mapAddressEntry &mapAddressEntry::r(read32_delegate func)
{
	assert(!func.isnull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.name();
	read32 = func;

	return *this;
}

mapAddressEntry &mapAddressEntry::r(read64_delegate func)
{
	assert(!func.isnull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.name();
	read64 = func;

	return *this;
}



mapAddressEntry &mapAddressEntry::w(write8_delegate func)
{
	assert(!func.isnull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.name();
	write8 = func;

	return *this;
}


mapAddressEntry &mapAddressEntry::w(write16_delegate func)
{
	assert(!func.isnull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.name();
	write16 = func;

	return *this;
}

mapAddressEntry &mapAddressEntry::w(write32_delegate func)
{
	assert(!func.isnull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.name();
	write32 = func;

	return *this;
}

mapAddressEntry &mapAddressEntry::w(write64_delegate func)
{
	assert(!func.isnull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.name();
	write64 = func;

	return *this;
}
