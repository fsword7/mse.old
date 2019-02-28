/*
 * maprom.h
 *
 *  Created on: Feb 9, 2019
 *      Author: Tim Stark
 */

#pragma once

#define ROM_TYPE_MASK				0x0000000F
#define 	ROM_TYPE_END			0
#define 	ROM_TYPE_REGION			1
#define 	ROM_TYPE_IMAGE			2
#define 	ROM_TYPE_FILL			3
#define 	ROM_TYPE_COPY			4
#define		ROM_TYPE_RELOAD			5
#define		ROM_TYPE_IGNORE			6
#define		ROM_TYPE_CONTINUE		7



#define ROM_REGION_WIDTH			0x00000030
#define 	ROM_REGION_8BIT			0x00000000
#define 	ROM_REGION_16BIT		0x00000010
#define 	ROM_REGION_32BIT		0x00000020
#define 	ROM_REGION_64BIT		0x00000030

#define ROM_REGION_ENDIAN			0x00000040
#define 	ROM_REGION_LE			0x00000000
#define 	ROM_REGION_BE			0x00000040

#define ROM_REGION_DATA				0x00000100
#define 	ROM_REGION_ROM			0x00000000

#define ROM_REGION_ERASEMASK		0x00000200
#define 	ROM_REGION_NOERASE		0x00000000
#define 	ROM_REGION_ERASE		0x00000200

#define ROMREGION_ERASEVALMASK			0x00FF0000
#define 	ROMREGION_ERASEVAL(val)		((((val) & 0xFF) << 16) | ROM_REGION_ERASE)
#define 	ROMREGION_ERASE00			ROM_REGION_ERASEVAL(0x00)
#define 	ROMREGION_ERASEFF			ROM_REGION_ERASEVAL(0xFF)

// per-ROM constants
#define ROM_OPTIONALMASK			0x00000010
#define 	ROM_REQUIRED			0x00000000
#define 	ROM_OPTIONAL			0x00000010

#define ROM_INHERITFLAGMASK			0x00000020
#define		ROM_INHERITFLAG			0x00000020

#define ROM_REVERSEMASK				0x00000040
#define		ROM_NOREVERSE			0x00000000
#define		ROM_REVERSE				0x00000040

#define ROM_GROUPSIZEMASK			0x00000F00
#define		ROM_GROUPSIZE(n)		((((n) - 1) & 0xF) << 8)
#define		ROM_GROUPBYTE			ROM_GROUPSIZE(1)
#define		ROM_GROUPWORD			ROM_GROUPSIZE(2)
#define		ROM_GROUPDWORD			ROM_GROUPSIZE(4)

#define ROM_SKIPMASK				0x0000F000
#define 	ROM_SKIP(n)				(((n) & 0xF) << 12)
#define 	ROM_NOSKIP				ROM_SKIP(0)

#define ROM_BITWIDTHMASK			0x000F0000
#define		ROM_BITWIDTH(n)			(((n) & 0xF) << 16)
#define		ROM_NIBBLE				ROM_BITWIDTH(4)
#define		ROM_BYTE				ROM_BITWIDTH(8)

#define ROM_BITSHIFTMASK			0x00F00000
#define		ROM_BITSHIFT(n)			(((n) & 0xF) << 20)
#define		ROM_NOSHIFT				ROM_BITSHIFT(0)
#define		ROM_NIBBLE_LO			ROM_BITSHIFT(0)
#define		ROM_NIBBLE_HI			ROM_BITSHIFT(4)

#define	ROM_BIOSFLAGMASK			0xFF0000000
#define		ROM_BIOS(n)				((((n) + 1) & 0xFF) << 24)

#define ROM_INHERITFLAGS			(ROM_GROUPMASK | ROM_SKIPMASK | ROM_REVERSEMASK | ROM_BITWIDTHMASK | ROM_BITSHIFTMASK | ROM_BIOSFLAGSMASK)


#define ROM_REGION(tag, length, flags)			{ tag, nullptr, 0, length, 0, (flags) | ROM_TYPE_REGION }
#define ROM_REGION16_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_REGION_BE)
#define ROM_REGION16_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_16BIT | ROM_REGION_LE)
#define ROM_REGION32_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_REGION_BE)
#define ROM_REGION32_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_32BIT | ROM_REGION_LE)
#define ROM_REGION64_BE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_REGION_BE)
#define ROM_REGION64_LE(tag, length, flags)		ROM_REGION(tag, length, (flags) | ROM_REGION_64BIT | ROM_REGION_LE)

#define ROM_LOAD(name, offset, length, hash, flags)	{ name, hash, offset, length, 0, ROM_TYPE_IMAGE | (flags) }

#define ROM_NAME(Name)		rom_##Name
#define ROM_END				{ nullptr, nullptr, 0, 0, 0, ROM_TYPE_END }

#define ROMENTRY_ISREGION(e)			((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_REGION)
#define ROMENTRY_ISEND(e)				((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_END)
#define ROMENTRY_ISFILE(e)				((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_IMAGE)
#define ROMENTRY_ISFILL(e)				((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_FILL)
#define ROMENTRY_ISCOPY(e)				((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_COPY)
#define ROMENTRY_ISIGNORE(e)			((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_IGNORE)
#define ROMENTRY_ISCONTINUE(e)			((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_CONTINUE)
#define ROMENTRY_ISRELOAD(e)			((ROM_GETFLAGS(e) & ROM_TYPE_MASK) == ROM_TYPE_RELOAD)
#define ROMENTRY_ISREGIONEND(e)			(ROMENTRY_ISREGION(e) || ROMENTRY_ISEND(e))

#define ROMREGION_GETNAME(e)			((e).name)
#define ROMREGION_GETOFFSET(e)			((e).offset)
#define ROMREGION_GETLENGTH(e)			((e).length)
#define ROMREGION_GETWIDTH(e)			(8 << ((ROM_GETFLAGS(e) & ROM_REGION_WIDTH) >> 4))
#define ROMREGION_GETDATATYPE(e)		(ROM_GETFLAGS(e) & ROM_REGION_DATA)
#define ROMREGION_GETERASEVAL(e)		((ROM_GETFLAGS(e) >> 16) & 0xFF)
#define ROMREGION_ISROMDATA(e)			(ROMREGION_GETDATATYPE(e) == ROM_REGION_ROM)
#define ROMREGION_ISBIGENDIAN(e)		((ROM_GETFLAGS(e) & ROM_REGION_ENDIAN) == ROM_REGION_BE)
#define ROMREGION_ISLITTLEENDIAN(e)		((ROM_GETFLAGS(e) & ROM_REGION_ENDIAN) == ROM_REGION_LE)
#define ROMREGION_ISERASE(e)			((ROM_GETFLAGS(e) & ROM_REGION_ERASEMASK) == ROM_REGION_ERASE)

#define ROM_GETNAME(e)					((e).name)
#define ROM_GETHASH(e)					((e).hash)
#define ROM_GETOFFSET(e)				((e).offset)
#define ROM_GETLENGTH(e)				((e).length)
#define ROM_GETVALUE(e)					((e).value)
#define ROM_GETFLAGS(e)					((e).flags)

#define ROM_GETGROUPSIZE(e)				(((ROM_GETFLAGS(e) & ROM_GROUPSIZEMASK) >> 8) + 1)
#define ROM_GETSKIP(e)					((ROM_GETFLAGS(e) & ROM_SKIPMASK) >> 12)
#define ROM_GETBITWIDTH(e)				((ROM_GETFLAGS(e) & ROM_BITWIDTHMASK) ? ((ROM_GETFLAGS(e) & ROM_BITWIDTHMASK) >> 16) : 8)
#define ROM_GETBITSHIFT(e)				((ROM_GETFLAGS(e) & ROM_BITSHIFTMASK) >> 20)
#define ROM_GETBIOSFLAGS(e)				((ROM_GETFLAGS(e) & ROM_BIOSFLAGSMASK) >> 24)

#define ROM_ISOPTIONAL(e)				((ROM_GETFLAGS(e) & ROM_OPTIONALMASK) == ROM_OPTONAL)
#define ROM_ISREVERSED(e)				((ROM_GETFLAGS(e) & ROM_REVERSEMASK) == ROM_REVERSE)
#define ROM_HASINHERITFLAG(e)			((ROM_GETFLAGS(e) & ROM_INHERITFLAGMASK) == ROM_INHERITFLAG)

struct romEntry_t
{
	const char 		*name;
	const char 		*hash;
	const uint32_t	offset;
	const uint32_t	length;
	const uint32_t	value;
	const uint32_t	flags;
};
