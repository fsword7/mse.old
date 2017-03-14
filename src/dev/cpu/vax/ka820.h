/*
 * ka820.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  VAX 82x0/83x0 series system
 *
 */

#pragma once

// System ID register definition
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |       5       |T|CPU rev|     Patch rev     |1|   uCode rev   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// T = 0 if VAX 8200/8300
//     1 if VAX 8250/8350

#define SID_ID      (5 << 24)
#define SID_8x50    (1u << 23)
#define SID_CPUREV  (0u << 19)
#define SID_PATREV  (0u << 9)
#define SID_MBO     (1u << 8)
#define SID_UCODE   0

// KA780 SID Serial Number
#define SID_SN      1
#define SID_SN_MASK ((1u << 12) - 1)

class ka820_cpuDevice : public vax_cpuDevice
{
public:
	ka820_cpuDevice();
	~ka820_cpuDevice();

	void reset();
	void execute();
};
