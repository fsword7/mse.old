/*
 * cvax.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX 78034 processor emulation
 *
 */

#pragma once

// CVAX System ID register definition
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      10       |           Reserved            |   uCode rev   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define SID_ID		(10 << 24)
#define SID_UCODE	6

#define CON_PWRUP   (3 << 8)

class cvax_cpuDevice : public vax_cpuDevice
{
public:
	cvax_cpuDevice();
	~cvax_cpuDevice();

	void reset();
	int  boot();
	void execute();

	static cvax_cpuDevice *create(sysDevice *sdev, std::string devName);

protected:
	// Read/write privileged register access
	uint32_t readpr(uint32_t pReg);
	void     writepr(uint32_t pReg, uint32_t data);

	// Instructions
	void mfpr();
	void mtpr();
};
