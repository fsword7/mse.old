/*
 * ka780.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 * VAX-11/780 series system
 *
 */

#pragma once

// System ID register definition
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |       1       |T|   ECO Level   |Plant|     Serial Number     |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// T = 0 if VAX-11/780
//     1 if VAX-11/785

#define SID_ID     (1 << 24)
#define SID_785    (1u << 23)
#define SID_ECO    (0u << 15)
#define SID_PLANT  (0u << 12)

// KA780 SID Serial Number
#define SID_SN      1
#define SID_SN_MASK ((1u << 12) - 1)

class ka780_cpuDevice : public vax_cpuDevice
{
public:
	ka780_cpuDevice();
	~ka780_cpuDevice();

	void reset();
	void execute();

};
