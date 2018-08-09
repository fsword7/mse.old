/*
 * vax.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Timothy Stark
 */

#pragma once

class vax_sysDevice : public sysDevice
{
public:
	vax_sysDevice();
	~vax_sysDevice();

	inline uint8_t *getMemory() const { return mem; }

	int setMemory(uint32_t size);
	int load(std::string fname, uint32_t sAddr);

	virtual uint32_t readio(cpuDevice *cpu, uint32_t pAddr, uint32_t acc) = 0;
	virtual void     writeio(cpuDevice *cpu, uint32_t pAddr, uint32_t data, uint32_t acc) = 0;

protected:
	uint32_t  memSize;
	uint8_t  *mem;

	dec::ctyDevice *cty;
};
