/*
 * cvax.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX processor emulation
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

class cvax_cpuDevice : public vax_cpuDevice
{
public:
	cvax_cpuDevice();
	~cvax_cpuDevice();

	void reset();
	void execute();
};
