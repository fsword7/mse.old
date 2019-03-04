/*
 * addrmap.h - Address space mapping
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

class mapAddress;

enum mapType {
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

struct mapHandler {
//	mapHandler() {
//		type = mapNone;
//		bits = 0;
//		tag  = nullptr;
//		name = nullptr;
//	}

	mapType		type  = mapNone;		// Mapping access type
	uint8_t		bits  = 0;			// Bit width (0 for default)
	const char	*tag  = nullptr;		// Tag name for named ports (I/O and banks)
	const char	*name = nullptr;	// Handler name
};

class mapAddressEntry {
public:
	mapAddressEntry(device_t &device, mapAddress &map, offs_t start, offs_t end);

	template <typename T, typename U>
	static std::enable_if_t<std::is_convertible<std::add_pointer_t<U>, std::add_pointer_t<T>>::value, T *>
		make_pointer(U &obj)
	{
		return &dynamic_cast<T &>(obj);
	}

	template <typename T, typename U>
	static std::enable_if_t<!std::is_convertible<std::add_pointer_t<U>, std::add_pointer_t<T>>::value, T *>
		make_pointer(U &obj)
	{
		return &dynamic_cast<T &>(obj);
	}

	// Mapping parameter setting function calls

	// RAM/ROM mapping
	mapAddressEntry &ram()		{ read.type = mapRAM; write.type = mapRAM; return *this; }
	mapAddressEntry &rom()		{ read.type = mapROM; write.type = mapNop; return *this; }
	mapAddressEntry &ronly()	{ read.type = mapRAM; return *this; }
	mapAddressEntry &wonly()	{ write.type = mapRAM; return *this; }

	// Non-existent access mapping
	mapAddressEntry &noprw()	{ read.type = mapNop; write.type = mapNop; return *this; }
	mapAddressEntry &nopr()		{ read.type = mapNop; return *this; }
	mapAddressEntry &nopw()		{ write.type = mapNop; return *this; }

	mapAddressEntry &mirror(offs_t bits) { adrMirror = bits; return *this; }
	mapAddressEntry &select(offs_t bits) { adrSelect = bits; return *this; }

	mapAddressEntry &region(tag_t *tag, offs_t offset = 0)
		{ tagRegion = tag; rgnOffset = offset; return *this; }
	mapAddressEntry &share(tag_t *tag) { tagShare = tag; return *this; }

	mapAddressEntry &mask(offs_t mask);

	mapAddressEntry &r(read8_delegate func);
	mapAddressEntry &r(read16_delegate func);
	mapAddressEntry &r(read32_delegate func);
	mapAddressEntry &r(read64_delegate func);

	mapAddressEntry &w(write8_delegate func);
	mapAddressEntry &w(write16_delegate func);
	mapAddressEntry &w(write32_delegate func);
	mapAddressEntry &w(write64_delegate func);


	// Implicit delegate calls
	template <typename T, typename Ret, typename... Params>
	mapAddressEntry &r(Ret (T::*read)(Params...), tag_t *readName)
	{
		return r(make_delegate(read, readName, device.tagName(), make_pointer<T>(device)));
	}

	template <typename T, typename Ret, typename... Params>
	mapAddressEntry &w(Ret (T::*write)(Params...), tag_t *writeName)
	{
		return w(make_delegate(write, writeName, device.tagName(), make_pointer<T>(device)));
	}

	template <typename T, typename rRet, typename... rParams, typename U, typename wRet, typename... wParams>
	mapAddressEntry &rw(rRet (T::*read)(rParams...), tag_t *readName, wRet (U::*write)(wParams...), tag_t *writeName)
	{
		r(make_delegate(read, readName, device.tagName(), make_pointer<T>(device)));
		w(make_delegate(write, writeName, device.tagName(), make_pointer<T>(device)));
		return *this;
	}

//	template <typename T, typename Ret, typename... Params>
//	mapAddressEntry &m(Ret (T::*map)(Params...), tag_t *mapName)
//	{
//		return *this;
//	}



	// Address entry information (public access)
	mapAddressEntry *mapNext;	// Point to the next entry
	mapAddress   	&map;		// Reference to address map database
	device_t		&device;	// Reference to base device

	// Address parameters (public address)
	offs_t		adrStart;	// Start address
	offs_t		adrEnd;		// End address
	offs_t		adrMask;	// Mask address bits
	offs_t		adrMirror;	// Mirror address bits
	offs_t		adrSelect;	// Select address bits

	mapHandler  read, write;	// Memory map handler for read and write access
	tag_t		*tagShare;		// Tag of shared map block
	tag_t		*tagRegion;		// Tag of region map block
	offs_t		rgnOffset;		// Offset from region base

	void		*memory;

	device_t		*submapDevice;
	mapConstructor	submapDelegate;

	// access handlers
	read8_delegate		read8;
	read16_delegate		read16;
	read32_delegate		read32;
	read64_delegate		read64;

	write8_delegate		write8;
	write16_delegate	write16;
	write32_delegate	write32;
	write64_delegate	write64;


};

class mapAddress {
	friend class mapAddressSpace;

public:
	mapAddress(device_t &device, int space);
	mapAddress(device_t &device, mapAddressEntry *entry);
	~mapAddress();

	mapAddressEntry &operator()(offs_t start, offs_t end);

	void setGlobalMask(offs_t mask) { gmask = mask; }

	void validate(int space) const;

	void importSubmaps(machine &sysMachine, device_t &owner, int dataWidth, endian_t endian);

	offs_t	gmask;	// Global bit mask

	uint64_t unmapValue;

private:
	device_t &device;
	int       adrSpace;
	std::vector<mapAddressEntry *> list;
};

