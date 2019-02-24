/*
 * maprom.h
 *
 *  Created on: Feb 9, 2019
 *      Author: Tim Stark
 */

#pragma once

#define ROM_TYPE_MASK		0x0000000F
#define ROM_TYPE_IMAGE		0
#define ROM_TYPE_REGION		1
#define ROM_TYPE_END		0xF

#define ROM_REGION_WIDTH	0x00000030
#define ROM_REGION_8BIT		0x00000000
#define ROM_REGION_16BIT	0x00000010
#define ROM_REGION_32BIT	0x00000020
#define ROM_REGION_64BIT	0x00000030

#define ROM_REGION_ENDIAN	0x00000040
#define ROM_REGION_LE		0x00000000
#define ROM_REGION_BE		0x00000040

#define ROM_REGION(tag, length, flags)			{ tag, nullptr, 0, length, (flags) | ROM_TYPE_REGION }
#define ROM_REGION16_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_REGION_BE)
#define ROM_REGION16_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_REGION_LE)
#define ROM_REGION32_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_REGION_BE)
#define ROM_REGION32_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_REGION_LE)
#define ROM_REGION64_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_REGION_BE)
#define ROM_REGION64_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_REGION_LE)

#define ROM_LOAD(name, offset, length, hash, flags)	{ name, hash, offset, length, ROM_TYPE_IMAGE | (flags) }

#define ROM_NAME(Name)		rom_##Name
#define ROM_END				{ nullptr, nullptr, 0, 0, ROM_TYPE_END }

#define ROMENTRY_ISREGION(e)		(((e).flags & ROM_TYPE_MASK) == ROM_TYPE_REGION)
#define ROMENTRY_ISEND(e)			(((e).flags & ROM_TYPE_MASK) == ROM_TYPE_END)

#define ROMREGION_GETNAME(e)		((e).name)
#define ROMREGION_GETLENGTH(e)		((e).length)

class romEntry
{
public:
	const char *name;
	const char *hash;
	uint32_t	offset;
	uint32_t	length;
	uint32_t	flags;
};
