/*
 * emumap_access.cpp
 *
 *  Created on: Mar 2, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/addrmap.h"
#include "emu/dimem.h"
#include "emu/machine.h"
#include "emu/exception.h"

#include "emu/map/hedr.h"
#include "emu/map/hedw.h"
#include "emu/map/heun.h"
#include "emu/map/hea.h"
#include "emu/map/hem.h"

// ***********************************************************

template <int dWidth, int aShift, int Endian>
class mapAddressSpaceAccess : public mapAddressSpace
{
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using thisType = mapAddressSpaceAccess<dWidth, aShift, Endian>;
	using nativeType = uintx_t;

	static constexpr uint32_t nativeBytes = 1 << dWidth;
	static constexpr uint32_t nativeBits = 8 * nativeBytes;
	static constexpr uint32_t nativeStep = aShift >= 0 ? nativeBytes << labs(aShift) : nativeBytes >> labs(aShift);
	static constexpr uint32_t nativeMask = nativeStep - 1;

	static constexpr offs_t offsetToByte(offs_t offset)
	{
		return aShift < 0 ? offset << labs(aShift) : offset >> labs(aShift);
	}

public:
	mapAddressSpaceAccess(mapMemoryManager &manager, di_memory &memory, int space, int addrWidth)
	: mapAddressSpace(manager, memory, space),
	  rootRead(nullptr), rootWrite(nullptr)
	{

		readUnmap = new mapHandlerReadUnmapped<dWidth, aShift, Endian>(this);
		writeUnmap = new mapHandlerWriteUnmapped<dWidth, aShift, Endian>(this);
		readNop = new mapHandlerReadNop<dWidth, aShift, Endian>(this);
		writeNop = new mapHandlerWriteNop<dWidth, aShift, Endian>(this);

		mapHandlerEntry::range r = { 0, 0xFFFFFFFF >> (32 - addrWidth) };

		switch (addrWidth) {

		case 1:
			rootRead  = new mapHandlerReadDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 2:
			rootRead  = new mapHandlerReadDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 3:
			rootRead  = new mapHandlerReadDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 4:
			rootRead  = new mapHandlerReadDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 5:
			rootRead  = new mapHandlerReadDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 6:
			rootRead  = new mapHandlerReadDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 7:
			rootRead  = new mapHandlerReadDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 8:
			rootRead  = new mapHandlerReadDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 9:
			rootRead  = new mapHandlerReadDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 10:
			rootRead  = new mapHandlerReadDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 11:
			rootRead  = new mapHandlerReadDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 12:
			rootRead  = new mapHandlerReadDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 13:
			rootRead  = new mapHandlerReadDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 14:
			rootRead  = new mapHandlerReadDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 15:
			rootRead  = new mapHandlerReadDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 16:
			rootRead  = new mapHandlerReadDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 17:
			rootRead  = new mapHandlerReadDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 18:
			rootRead  = new mapHandlerReadDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 19:
			rootRead  = new mapHandlerReadDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 20:
			rootRead  = new mapHandlerReadDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 21:
			rootRead  = new mapHandlerReadDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 22:
			rootRead  = new mapHandlerReadDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 23:
			rootRead  = new mapHandlerReadDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 24:
			rootRead  = new mapHandlerReadDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 25:
			rootRead  = new mapHandlerReadDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 26:
			rootRead  = new mapHandlerReadDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 27:
			rootRead  = new mapHandlerReadDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 28:
			rootRead  = new mapHandlerReadDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 29:
			rootRead  = new mapHandlerReadDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 30:
			rootRead  = new mapHandlerReadDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 31:
			rootRead  = new mapHandlerReadDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 32:
			rootRead  = new mapHandlerReadDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapHandlerWriteDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		default:
			std::cerr << "Unhandled address bus width: " << addrWidth << std::endl;
		}
	}

	nativeType readNative(offs_t offset, nativeType mask)
	{
		return rootRead->read(offset, mask);
	}

	nativeType readNative(offs_t offset)
	{
		return rootRead->read(offset, uintx_t(0xFFFFFFFFFFFFFFFFu));
	}

	nativeType writeNative(offs_t offset, nativeType data, nativeType mask)
	{
		return rootWrite->write(offset, data, mask);
	}

	nativeType writeNative(offs_t offset, nativeType data)
	{
		return rootWrite->write(offset, data, uintx_t(0xFFFFFFFFFFFFFFFFu));
	}

	// read accessors
//	uint8_t read8(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16(offs_t address, uint16_t mask) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint16_t read16u(offs_t address, uint16_t mask) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32(offs_t address, uint32_t mask) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint32_t read32u(offs_t address, uint32_t mask) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64(offs_t address, uint64_t mask) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64u(offs_t address) override
//		{ address &= addrMask; return 0; }
//	uint64_t read64u(offs_t address, uint64_t mask) override
//		{ address &= addrMask; return 0; }

	// write accessors
//	void write8(offs_t address, uint8_t data) override
//		{ address &= addrMask;  }
//	void write16(offs_t address, uint16_t data) override
//		{ address &= addrMask;  }
//	void write16(offs_t address, uint16_t data, uint16_t mask) override
//		{ address &= addrMask;  }
//	void write16u(offs_t address, uint16_t data) override
//		{ address &= addrMask;  }
//	void write16u(offs_t address, uint16_t data, uint16_t mask) override
//		{ address &= addrMask;  }
//	void write32(offs_t address, uint32_t data) override
//		{ address &= addrMask;  }
//	void write32(offs_t address, uint32_t data, uint32_t mask) override
//		{ address &= addrMask;  }
//	void write32u(offs_t address, uint32_t data) override
//		{ address &= addrMask;  }
//	void write32u(offs_t address, uint32_t data, uint32_t mask) override
//		{ address &= addrMask;  }
//	void write64(offs_t address, uint64_t data) override
//		{ address &= addrMask;  }
//	void write64(offs_t address, uint64_t data, uint64_t mask) override
//		{ address &= addrMask;  }
//	void write64u(offs_t address, uint64_t data) override
//		{ address &= addrMask;  }
//	void write64u(offs_t address, uint64_t data, uint64_t mask) override
//		{ address &= addrMask;  }


//	// static access to functions
//	static uint8_t read8s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint16_t read16s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint16_t read16ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//	static uint32_t read32s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint32_t read32ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//	static uint64_t read64s(type &space, offs_t address)
//	{ address &= space.addrMask; return 0; }
//	static uint64_t read64ms(type &space, offs_t address, offs_t mask)
//	{ address &= space.addrMask; return 0; }
//
//	static void write8s(type &space, offs_t address, uint8_t data)
//	{ address &= space.addrMask;  }
//	static void write16s(type &space, offs_t address, uint16_t data)
//	{ address &= space.addrMask;  }
//	static void write16ms(type &space, offs_t address, uint16_t data, offs_t mask)
//	{ address &= space.addrMask;  }
//	static void write32s(type &space, offs_t address, uint32_t data)
//	{ address &= space.addrMask;  }
//	static void write32ms(type &space, offs_t address, uint32_t data, offs_t mask)
//	{ address &= space.addrMask;  }
//	static void write64s(type &space, offs_t address, uint64_t data)
//	{ address &= space.addrMask;  }
//	static void write64ms(type &space, offs_t address, uint64_t data, offs_t mask)
//	{ address &= space.addrMask;  }

	void setup_unmap_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type, bool quiet)
	{
		cty.printf("%s: %s space - Unmapped %08X-%08X Mirror=%08X, Access=%s, %s\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				(type == rwType::READ) ? "read" :
				(type == rwType::WRITE) ? "write" :
				(type == rwType::RW) ? "read/write" : "???",
				quiet ? "quiet" : "normal");

		offs_t nstart, nend, nmirror, nmask;
		checkOptimizeMirror(cty, "setup_unmap_generic", adrStart, adrEnd, adrMirror, nstart, nend, nmirror, nmask);

		// Address space - read access
		if (type == rwType::READ || type == rwType::RW)
		{
			auto handler = static_cast<mapHandlerRead<dWidth, aShift, Endian> *>(quiet ? readNop : readUnmap);
			handler->ref();
			rootRead->populate(cty, nstart, nend, nmirror, handler);
		}

		// Address space - write access
		if (type == rwType::WRITE || type == rwType::RW)
		{
			auto handler = static_cast<mapHandlerWrite<dWidth, aShift, Endian> *>(quiet ? writeNop : writeUnmap);
			handler->ref();
			rootWrite->populate(cty, nstart, nend, nmirror, handler);
		}

//		invalidate_caches(type);
	}

	void setup_ram_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type, void *base)
	{
		cty.printf("%s: %s space - RAM/ROM %08X-%08X Mirror=%08X, Access=%s, %p\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				(type == rwType::READ) ? "read" :
				(type == rwType::WRITE) ? "write" :
				(type == rwType::RW) ? "read/write" : "???",
				base);

		offs_t nstart, nend, nmirror, nmask;
		checkOptimizeMirror(cty, "setup_ram_generic", adrStart, adrEnd, adrMirror, nstart, nend, nmirror, nmask);

		// Address space - read access
		if (type == rwType::READ || type == rwType::RW)
		{
			mapMemoryBank &bank = allocateBank(nullptr, adrStart, adrEnd, adrMirror, rwType::READ);
			if (base != nullptr)
				bank.setBase(base);

//			if (bank.base() == nullptr)
//			{
//				void *backing = findBackingMemory(cty, adrStart, adrEnd);
//				if (backing != nullptr)
//					bank.setBase(backing);
//			}
//
//			if (bank.base() == nullptr)
//			{
//			}
//
			auto handler = new mapHandlerReadMemory<dWidth, aShift, Endian>(this);
//			if (bank.base())
//				handler->setBase(static_cast<uintx_t *>(bank.base()));
//			else {
//
//			}

			handler->setAddressInfo(nstart, nend);
			rootRead->populate(cty, nstart, nend, nmirror, handler);
		}

		// Address space - write access
		if (type == rwType::WRITE || type == rwType::RW)
		{
			mapMemoryBank &bank = allocateBank(nullptr, adrStart, adrEnd, adrMirror, rwType::WRITE);
			if (base != nullptr)
				bank.setBase(base);

//			if (bank.base() == nullptr)
//			{
//				void *backing = findBackingMemory(cty, adrStart, adrEnd);
//				if (backing != nullptr)
//					bank.setBase(backing);
//			}
//
//			if (bank.base() == nullptr)
//			{
//			}
//
			auto handler = new mapHandlerWriteMemory<dWidth, aShift, Endian>(this);
//			if (bank.base())
//				handler->setBase(static_cast<uintx_t *>(bank.base()));
//			else {
//
//			}

			handler->setAddressInfo(nstart, nend);
			rootWrite->populate(cty, nstart, nend, nmirror, handler);
		}

//		invalidate_caches(type);
	}

	void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, std::string rtag, std::string wtag)
	{
		cty.printf("%s: %s space - bank %08X-%08X Mirror=%08X, read=%s, write=%s\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				!rtag.empty() ? rtag.c_str() : "(none)",
				!wtag.empty() ? wtag.c_str() : "(none)");

		offs_t nstart, nend, nmirror, nmask;
		checkOptimizeMirror(cty, "setup_bank_generic", adrStart, adrEnd, adrMirror, nstart, nend, nmirror, nmask);

		// Address space - read access
		if (rtag != "")
		{
			mapMemoryBank &bank = allocateBank(rtag.c_str(), adrStart, adrEnd, adrMirror, rwType::READ);

			auto handler = new mapHandlerReadMemoryBank<dWidth, aShift, Endian>(this, bank);
			handler->setAddressInfo(nstart, nend);
			rootRead->populate(cty, nstart, nend, nmirror, handler);
		}

		// Address space - write access
		if (wtag != "")
		{
			mapMemoryBank &bank = allocateBank(wtag.c_str(), adrStart, adrEnd, adrMirror, rwType::WRITE);

			auto handler = new mapHandlerWriteMemoryBank<dWidth, aShift, Endian>(this, bank);
			handler->setAddressInfo(nstart, nend);
			rootWrite->populate(cty, nstart, nend, nmirror, handler);
		}

//		invalidate_caches((rtag != "") ? (wtag != "") ? rwTypw::RW : rwType::READ : rwType::WRITE);
	}

	void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, mapMemoryBank *rbank, mapMemoryBank *wbank)
	{
		cty.printf("%s: %s space - bank %08X-%08X Mirror=%08X, read=%s, write=%s\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				(rbank != nullptr) ? rbank->tagName() : "(none)",
				(wbank != nullptr) ? wbank->tagName() : "(none)");

		offs_t nstart, nend, nmirror, nmask;
		checkOptimizeMirror(cty, "setup_bank_generic", adrStart, adrEnd, adrMirror, nstart, nend, nmirror, nmask);

		// Address space - read access
		if (rbank != nullptr)
		{
			auto handler = new mapHandlerReadMemoryBank<dWidth, aShift, Endian>(this, *rbank);
			handler->setAddressInfo(nstart, nend);
			rootRead->populate(cty, nstart, nend, nmirror, handler);
		}

		// Address space - write access
		if (wbank != nullptr)
		{
			auto handler = new mapHandlerWriteMemoryBank<dWidth, aShift, Endian>(this, *wbank);
			handler->setAddressInfo(nstart, nend);
			rootWrite->populate(cty, nstart, nend, nmirror, handler);
		}

//		invalidate_caches((rbank != nullptr) ? (wbank != nullptr) ? rwTypw::RW : rwType::READ : rwType::WRITE);
	}

	mapHandlerRead<dWidth, aShift, Endian>  *rootRead;
	mapHandlerWrite<dWidth, aShift, Endian> *rootWrite;
};

// ************************************************************************

void mapMemoryManager::allocate(const cty_t &cty, di_memory &memory)
{
	cty.printf("%s: Allocating address space (%d space%s)\n",
		memory.getDevice()->tagName(), memory.mapConfigCount(),
		memory.mapConfigCount() != 1 ? "s" : "");

	for (int space = 0; space < memory.mapConfigCount(); space++) {
		const mapAddressConfig *config = memory.getAddressSpaceConfig(space);
		if (config != nullptr) {
			cty.printf("%s: Allocating address space...\n", memory.getDevice()->tagName());

			switch( config->data_width() | (4 - config->address_shift())) {
			// 8-bit address width
			case 8|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<0, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<0, 0, endianLittle>>(*this, space);
				break;

			// 16-bit address width
			case 16|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<1, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<1, 0, endianLittle>>(*this, space);
				break;
			case 16|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<1, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<1, -1, endianLittle>>(*this, space);
				break;

			// 32-bit address width
			case 32|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, 0, endianLittle>>(*this, space);
				break;
			case 32|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, -1, endianLittle>>(*this, space);
				break;
			case 32|(4-2):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<2, -2, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<2, -2, endianLittle>>(*this, space);
				break;

			// 64-bit address width
			case 64|(4-0):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, 0, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, 0, endianLittle>>(*this, space);
				break;
			case 64|(4-1):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -1, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -1, endianLittle>>(*this, space);
				break;
			case 64|(4-2):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -2, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -2, endianLittle>>(*this, space);
				break;
			case 64|(4-3):
				if (config->endian() == endianBig)
					memory.allocate<mapAddressSpaceAccess<3, -3, endianBig>>(*this, space);
				else
					memory.allocate<mapAddressSpaceAccess<3, -3, endianLittle>>(*this, space);
				break;

			default:
				throw mseFatalError("Address Space: Invalid address width %d shift %d",
					config->address_width(), config->address_shift());
			}
		}
	}
}




