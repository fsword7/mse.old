/*
 * hem.h - memory access handlers
 *
 *  Created on: Mar 4, 2019
 *      Author: fswor
 */

#pragma once

template <int dWidth, int aShift, int Endian>
class mapHandlerReadMemory : public mapHandlerReadAddress<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using inh = mapHandlerReadAddress<dWidth, aShift, Endian>;

	mapHandlerReadMemory(mapAddressSpace *space)
	: mapHandlerReadAddress<dWidth, aShift, Endian>(space, 0),
	  baseData(nullptr)
	  { }
	~mapHandlerReadMemory() = default;

	std::string name() const override
	{
		return "memory";
	}

	inline void setBase(uintx_t *base) { baseData = base; }

	uintx_t read(offs_t offset, uintx_t mask) override
	{
//		printf("%s: Data width=%d Address=%08X Mask=%08X -> %08X (%08X %08X %d)\n",
//			name().c_str(), dWidth, offset, mask,
//			(((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)),
//			inh::baseAddress, inh::maskAddress, (dWidth+aShift));
		return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

	void *getAccess(offs_t offset) const override
	{
		return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

private:
	uintx_t *baseData;
};

template <int dWidth, int aShift, int Endian>
class mapHandlerWriteMemory : public mapHandlerWriteAddress<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using inh = mapHandlerWriteAddress<dWidth, aShift, Endian>;

	mapHandlerWriteMemory(mapAddressSpace *space)
	: mapHandlerWriteAddress<dWidth, aShift, Endian>(space, 0),
	  baseData(nullptr)
	  { }
	~mapHandlerWriteMemory() = default;

	std::string name() const override
	{
		return "memory";
	}

	inline void setBase(uintx_t *base) { baseData = base; }

	void write(offs_t offset, uintx_t data, uintx_t mask) override
	{
		offs_t off = ((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift);
		baseData[off] = (baseData[off] & ~mask) | (data & mask);
	}

	void *getAccess(offs_t offset) const override
	{
		return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

private:
	uintx_t *baseData;
};



template <int dWidth, int aShift, int Endian>
class mapHandlerReadMemoryBank : public mapHandlerReadAddress<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using inh = mapHandlerReadAddress<dWidth, aShift, Endian>;

	mapHandlerReadMemoryBank(mapAddressSpace *space, mapMemoryBank &bank)
	: mapHandlerReadAddress<dWidth, aShift, Endian>(space, 0),
	  bank(bank)
	  { }
	~mapHandlerReadMemoryBank() = default;

	std::string name() const override
	{
		return bank.tagName();
	}

	uintx_t read(offs_t offset, uintx_t mask) override
	{
//		printf("%s: Data width=%d Address=%08X Mask=%08X -> %08X (%08X %08X %d)\n",
//			name().c_str(), dWidth, offset, mask,
//			(((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)),
//			inh::baseAddress, inh::maskAddress, (dWidth+aShift));
		return static_cast<uintx_t *>(bank.base())[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

	void *getAccess(offs_t offset) const override
	{
		return &static_cast<uintx_t *>(bank.base())[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

private:
	mapMemoryBank &bank;
};

template <int dWidth, int aShift, int Endian>
class mapHandlerWriteMemoryBank : public mapHandlerWriteAddress<dWidth, aShift, Endian>
{
public:
	using uintx_t = typename mapHandlerSize<dWidth>::uintx_t;
	using inh = mapHandlerWriteAddress<dWidth, aShift, Endian>;

	mapHandlerWriteMemoryBank(mapAddressSpace *space, mapMemoryBank &bank)
	: mapHandlerWriteAddress<dWidth, aShift, Endian>(space, 0),
	  bank(bank)
	  { }
	~mapHandlerWriteMemoryBank() = default;

	std::string name() const override
	{
		return bank.tagName();
	}

	void write(offs_t offset, uintx_t data, uintx_t mask) override
	{
		offs_t off = ((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift);
		static_cast<uintx_t *>(bank.base())[off] = (static_cast<uintx_t *>(bank.base())[off] & ~mask) | (data & mask);
	}

	void *getAccess(offs_t offset) const override
	{
		return &static_cast<uintx_t *>(bank.base())[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
	}

private:
	mapMemoryBank &bank;
};
