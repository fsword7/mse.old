/*
 * ka650.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  KA650/KA655 CVAX processor emulation
 *
 *
 */

#pragma once

// System ID register definition
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      10       |                                               |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define SID_ID	(10 << 24)
#define SID_REV	6

class ka650_cpuDevice : public vax_cpuDevice
{
public:
	ka650_cpuDevice();
	~ka650_cpuDevice();

	void reset();

};
