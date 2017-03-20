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

	int load(std::string fname);

protected:
	uint32_t  memSize;
	uint8_t  *mem;
};
