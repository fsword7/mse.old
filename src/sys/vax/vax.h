/*
 * vax.h
 *
 *  Created on: Mar 18, 2017
 *      Author: fswor
 */

#pragma once

class vax_sysDevice : public Device
{
public:
	vax_sysDevice();
	~vax_sysDevice();

	inline uint8_t *getMemory() const { return mem; }

	int setMemory(uint32_t size);
	int load(std::string fname);
	int dump(uint32_t *sAddr, uint32_t eAddr, uint32_t sw);

protected:
	uint32_t  memSize;
	uint8_t  *mem;
};
