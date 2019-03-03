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

		readUnmap = new mapReadHandlerUnmapped<dWidth, aShift, Endian>(this);
		writeUnmap = new mapWriteHandlerUnmapped<dWidth, aShift, Endian>(this);
		readNop = new mapReadHandlerNop<dWidth, aShift, Endian>(this);
		writeNop = new mapWriteHandlerNop<dWidth, aShift, Endian>(this);

		mapHandlerEntry::range r = { 0, 0xFFFFFFFF >> (32 - addrWidth) };

		switch (addrWidth) {

		case 1:
			rootRead  = new mapReadHandlerDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<1, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 2:
			rootRead  = new mapReadHandlerDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<2, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 3:
			rootRead  = new mapReadHandlerDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<3, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 4:
			rootRead  = new mapReadHandlerDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<4, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 5:
			rootRead  = new mapReadHandlerDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<5, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 6:
			rootRead  = new mapReadHandlerDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<6, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 7:
			rootRead  = new mapReadHandlerDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<7, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 8:
			rootRead  = new mapReadHandlerDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<8, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 9:
			rootRead  = new mapReadHandlerDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<9, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 10:
			rootRead  = new mapReadHandlerDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<10, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 11:
			rootRead  = new mapReadHandlerDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<11, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 12:
			rootRead  = new mapReadHandlerDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<12, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 13:
			rootRead  = new mapReadHandlerDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<13, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 14:
			rootRead  = new mapReadHandlerDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<14, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 15:
			rootRead  = new mapReadHandlerDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<15, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 16:
			rootRead  = new mapReadHandlerDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<16, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 17:
			rootRead  = new mapReadHandlerDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<17, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 18:
			rootRead  = new mapReadHandlerDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<18, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 19:
			rootRead  = new mapReadHandlerDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<19, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 20:
			rootRead  = new mapReadHandlerDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<20, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 21:
			rootRead  = new mapReadHandlerDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<21, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 22:
			rootRead  = new mapReadHandlerDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<22, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 23:
			rootRead  = new mapReadHandlerDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<23, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 24:
			rootRead  = new mapReadHandlerDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<24, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 25:
			rootRead  = new mapReadHandlerDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<25, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 26:
			rootRead  = new mapReadHandlerDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<26, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 27:
			rootRead  = new mapReadHandlerDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<27, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 28:
			rootRead  = new mapReadHandlerDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<28, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 29:
			rootRead  = new mapReadHandlerDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<29, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 30:
			rootRead  = new mapReadHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<30, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 31:
			rootRead  = new mapReadHandlerDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<31, dWidth, aShift, Endian>(this, r, nullptr);
			break;

		case 32:
			rootRead  = new mapReadHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
			rootWrite = new mapWriteHandlerDispatch<32, dWidth, aShift, Endian>(this, r, nullptr);
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

	}

	void setup_ram_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, rwType type, void *base)
	{
		cty.printf("%s: %s space - RAM/ROM %08X-%08X Mirror=%08X, Access=%s, %p\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				(type == rwType::READ) ? "read" :
				(type == rwType::WRITE) ? "write" :
				(type == rwType::RW) ? "read/write" : "???",
				base);

	}

	void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, std::string rtag, std::string wtag)
	{
		cty.printf("%s: %s space - bank %08X-%08X Mirror=%08X, read=%s, write=%s\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				!rtag.empty() ? rtag.c_str() : "(none)",
				!wtag.empty() ? wtag.c_str() : "(none)");

	}

	void setup_bank_generic(const cty_t &cty, offs_t adrStart, offs_t adrEnd, offs_t adrMirror, mapMemoryBank *rbank, mapMemoryBank *wbank)
	{
		cty.printf("%s: %s space - bank %08X-%08X Mirror=%08X, read=%s, write=%s\n",
				device.tagName(), name, adrStart, adrEnd, adrMirror,
				(rbank != nullptr) ? rbank->tagName() : "(none)",
				(wbank != nullptr) ? wbank->tagName() : "(none)");

	}

	mapReadHandlerEntry<dWidth, aShift, Endian>  *rootRead;
	mapWriteHandlerEntry<dWidth, aShift, Endian> *rootWrite;
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

			switch( config->address_width() | (4 - config->address_shift())) {
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




