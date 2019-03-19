/*
 * device.cpp
 *
 *  Created on: Jan 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/sysconfig.h"
#include "emu/maprom.h"
#include "emu/device.h"
#include "emu/devauto.h"
#include "emu/machine.h"
#include "emu/validity.h"

device_t::device_t(const system_config &config, const device_type_base &type, tag_t *tag, device_t *owner, uint64_t clock)
: type(type), devOwner(owner), devNext(nullptr),
  romEntries(nullptr),
  stagName(tag), devName(nullptr),
  sysConfig(config),
  system(nullptr)
{
//	romEntries.clear();
	acList.clear();
}

device_t::~device_t()
{
}

// When configuration of device is complete,
// execute that to finish configuration.
void device_t::completeConfig()
{

	for (device_interface *intf : interfaces()) {
		std::cout << intf->type() << ": Completeing configuration..." << std::endl;
		intf->intfCompleteConfig();
	}

	devCompleteConfig();
}

void device_t::configure(system_config &config)
{
	assert(&sysConfig == &config);

	// Assign current configuring device and
	// start configuration process.
	config.begin(this);
	devConfigure(config);

//	for(devauto_base *autodev : acList)
//		autodev->endConfig();


}

// Start all devices as initialization
void device_t::start()
{
	//	for (device_interface *intf : interfaces())
	//		intf->intfValidate(valid);

	// Specific-device initialization
	devStart();

}

// Stop all devices
void device_t::stop()
{
	//	for (device_interface *intf : interfaces())
	//		intf->intfValidate(valid);

	// Specific-device initialization
	devStop();

}

// Reset all devices
void device_t::reset()
{
//	for (device_interface *intf : interfaces())
//		intf->intfValidate(valid);

	// Reset this device first
	devReset();

	// Reset all child devices
	for (device_t &child : devices())
		child.reset();

//	devResetAfterChildren();

//	for (device_interface *intf : interfaces())
//		intf->intfValidate(valid);
}

devauto_base *device_t::register_device(devauto_base *autodev)
{
	devauto_base *last = nullptr;

	// add device to auto device list
	if (acList.size() > 0)
		last = acList.back();
	acList.push_back(autodev);

	return last;
}

void device_t::resolvePreMap()
{
}

void device_t::resolvePostMap()
{
}

void device_t::validate(validity_checker &valid) const
{
	for (device_interface *intf : interfaces())
		intf->intfValidate(valid);

	devValidate(valid);
}


void device_t::devConfigure(system_config &config)
{
	// Do nothing by default
}

void device_t::devCompleteConfig()
{
	// Do nothing by default
}

const romEntry_t *device_t::devGetROMRegion()
{
	// Do nothing by default
	return nullptr;
}

void device_t::devValidate(validity_checker &valid) const
{
	// Do nothing by default
}

//void device_t::devStart()
//{
//	// Do nothing by default
//}

void device_t::devStop()
{
	// Do nothing by default
}

void device_t::devReset()
{
	// Do nothing by default
}

// ********************************************************

//std::vector<const romEntry *> device_t::romBuildRegions()
//{
//	const romEntry_t *entries;
//
//	static const romEntry_t end = ROM_END;
//
//	if ((entries = devGetROMRegion()) != nullptr) {
//		int idx = 0;
//		do {
//			romEntries.push_back(&entries[idx]);
////			std::cout << "Count ROM entry " << idx << "..." << std::endl;
//		} while (!ROMENTRY_ISEND(entries[idx++]));
//	} else {
//		// Device does not have ROM entries
//		// Just create END entry.
//		romEntries.push_back(&end);
//	}
//	return romEntries;
//}

//std::vector<const romEntry *> device_t::romGetRegions()
//{
//	if (romEntries.empty())
//		romBuildRegions();
//	return romEntries;
//}

const romEntry_t *device_t::romGetRegions()
{
	static const romEntry_t rom_empty[] = { ROM_END };

	if (romEntries == nullptr) {
		romEntries = devGetROMRegion();
		if (romEntries == nullptr)
			romEntries = rom_empty;
	}

	return romEntries;
}


mapMemoryRegion *device_t::mapGetMemoryRegion(std::string tag) const
{
	auto search = system->memory().regions().find(tag);
	if (search != system->memory().regions().end())
		return search->second;
	else
		return nullptr;
}

mapMemoryBank *device_t::mapGetMemoryBank(std::string tag) const
{
	return nullptr;
}

mapMemoryShare *device_t::mapGetMemoryShare(std::string tag) const
{
	return nullptr;
}

// ********************************************************

device_interface::device_interface(device_t *dev, tag_t *name)
: next(nullptr), device(dev), typeName(name)
{
	device_t::interface_list &list = dev->interfaces();

	// Add interface into device's interface list.
	if (list.size() > 0)
		list.last()->next = this;
	list.add(this);
}

device_interface::~device_interface()
{
}

void device_interface::intfCompleteConfig()
{
	// Do nothing by default
}

void device_interface::intfValidate(validity_checker &valid) const
{
	// Do nothing by default
}
