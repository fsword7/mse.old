/*
 * maprom.h
 *
 *  Created on: Feb 9, 2019
 *      Author: Tim Stark
 */

#pragma once

enum romFlags {
	// Region Type Mask
	romTypeMask		= 0x0000000F,
	romTypeROM		= 0,
	romTypeRegion,
	romTypeEnd,

	romTypeCartridge,
	romTypeCount,

	// Region Width Mask
	romRegionWidth	= 0x000000F0,
	romRegion8bit	= 0x00000000,
	romRegion16bit	= 0x00000010,
	romRegion32bit	= 0x00000020,
	romRegion64bit	= 0x00000030,

	// Region Endian Mask
	romEndianMask	= 0x00000100,
	romEndianLE		= 0x00000000,
	romEndianBE		= 0x00000100,

};

#define ROM_REGION_WIDTH	0x00000004
#define ROM_REGION_8BIT		0x00000000
#define ROM_REGION_16BIT	0x00000001
#define ROM_REGION_32BIT	0x00000002
#define ROM_REGION_64BIT	0x00000003

#define ROM_REGION_ENDIAN	0x80000000
#define ROM_REGION_LE		0x00000000
#define ROM_REGION_BE		0x80000000

#define ROM_REGION(tag, length, flags)			{ tag, nullptr, 0, length, (flags) | romTypeRegion }
#define ROM_REGION16_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_ENDIAN_BE)
#define ROM_REGION16_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_ENDIAN_LE)
#define ROM_REGION32_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_ENDIAN_BE)
#define ROM_REGION32_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_ENDIAN_LE)
#define ROM_REGION64_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_ENDIAN_BE)
#define ROM_REGION64_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_ENDIAN_LE)


#define ROM_NAME(Name)		rom_##Name
#define ROM_START(Name)		static const mapRomEntry ROM_NAME(Name)[] = {
#define ROM_END				{ nullptr, nullptr, 0, 0, romTypeEnd } };

class mapRomEntry
{
public:
	const char *name;
	const char *hash;
	uint32_t	offset;
	uint32_t	length;
	romFlags	flags;
};
