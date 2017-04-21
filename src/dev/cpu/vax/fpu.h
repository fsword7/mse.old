/*
 * float.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Tim Stark
 *
 *  Float instructions
 *
 */

#pragma once

// Result codes
#define VFP_OK		0 // Successful
#define VFP_FAULT	1 // Fault
#define VFP_UNFL	2 // Underflow Error, FP = 0
#define VFP_OVFL	3 // Overflow Error
#define VFP_DZRO	4 // Divide by Zero

// Word-swapped floating
#define FP_SWAP(fp)		((ZXTL(fp) >> 16) | (ZXTL(fp) << 16))

#define FP_SIGN			0x00008000 // sign bit in word-swapped floating value

#define FP_P_NORM		63
#define FP_NORM			0x8000000000000000ULL
#define SFP_ROUND		0x0000008000000000ULL
#define DFP_ROUND		0x0000000000000080ULL
#define GFP_ROUND		0x0000000000000400ULL
#define HFP_ROUND		0x0000000000004000ULL

// Floating Format Definitions

// F_Floating Format (Single Precision)
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|  Exponential  |  Fraction   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define SFP_P_EXP		7
#define SFP_M_EXP		0xFF
#define SFP_BIAS		0x80
#define SFP_SIGN		FP_SIGN
#define SFP_HBIT		(1u << SFP_P_EXP)
#define SFP_GUARD		(15 - SFP_P_EXP)
#define SFP_EXP			(SFP_M_EXP << SFP_P_EXP)

#define SFP_GETEXP(fp)	(((fp) >> SFP_P_EXP) & SFP_M_EXP)
#define SFP_SETEXP(fp)	(((fp) & SFP_M_EXP) << SFP_P_EXP)
#define SFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(SFP_SIGN|SFP_EXP)) | SFP_HBIT)
#define SFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(SFP_SIGN|SFP_EXP|SFP_HBIT))

// D_Floating Format (Double Precision)
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|  Exponential  |  Fraction   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define DFP_P_EXP		7
#define DFP_M_EXP		0xFF
#define DFP_BIAS		0x80
#define DFP_SIGN		FP_SIGN
#define DFP_HBIT		(1u << DFP_P_EXP)
#define DFP_GUARD		(15 - DFP_P_EXP)
#define DFP_EXP			(DFP_M_EXP << DFP_P_EXP)

#define DFP_GETEXP(fp)	(((fp) >> DFP_P_EXP) & DFP_M_EXP)
#define DFP_SETEXP(fp)	(((fp) & DFP_M_EXP) << DFP_P_EXP)
#define DFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(DFP_SIGN|DFP_EXP)) | DFP_HBIT)
#define DFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(DFP_SIGN|DFP_EXP|DFP_HBIT))

// G_Floating Format Definitions
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|      Exponential    |       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define GFP_P_EXP		4
#define GFP_M_EXP		0x7FF
#define GFP_BIAS		0x400
#define GFP_SIGN		FP_SIGN
#define GFP_HBIT		(1u << GFP_P_EXP)
#define GFP_GUARD		(15 - GFP_P_EXP)
#define GFP_EXP			(GFP_M_EXP << GFP_P_EXP)

#define GFP_GETEXP(fp)	(((fp) >> GFP_P_EXP) & GFP_M_EXP)
#define GFP_SETEXP(fp)	(((fp) & GFP_M_EXP) << GFP_P_EXP)
#define GFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(GFP_SIGN|GFP_EXP)) | GFP_HBIT)
#define GFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(GFP_SIGN|GFP_EXP|GFP_HBIT))

// H_floating Format Definitions
//
// |<--------- Low-Order --------->|<-------- High-Order --------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Fraction            |S|        Exponential          |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
// |                          Fraction (Low)                       |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define HFP_P_EXP		0
#define HFP_P_NORM		127
#define HFP_M_EXP		0x7FFF
#define HFP_BIAS		0x4000
#define HFP_SIGN		FP_SIGN
#define HFP_HBIT		(1u << HFP_P_EXP)
#define HFP_GUARD		(15 - HFP_P_EXP)
#define HFP_EXP			(HFP_M_EXP << HFP_P_EXP)

#define HFP_GETEXP(fp)	(((fp) >> HFP_P_EXP) & HFP_M_EXP)
#define HFP_SETEXP(fp)	(((fp) & HFP_M_EXP) << HFP_P_EXP)
#define HFP_GETFRAC(fp)	FP_SWAP(((fp) & ~(HFP_SIGN|HFP_EXP)) | HFP_HBIT)
#define HFP_SETFRAC(fp)	(FP_SWAP(fp) & ~(HFP_SIGN|HFP_EXP|HFP_HBIT))


// Set N and Z flag
#define SetNZ_FP(cc, res, flg) \
	if ((res) & FP_SIGN)  cc = CC_N | (flg); \
	else if ((res) == 0)  cc = CC_Z | (flg); \
	else                  cc = (flg);


class sfp_t {
public:
	sfp_t(uint32_t fp);
	~sfp_t();

	int unpack(uint32_t fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};

class dfp_t {
public:
	dfp_t(uint32_t *fp);
	~dfp_t();

	int unpack(uint32_t *fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};

class gfp_t {
public:
	gfp_t(uint32_t *fp);
	~gfp_t();

	int unpack(uint32_t *fp);
	int pack(uint32_t *fp);
	void normalize();
	void convert(int32_t val);

protected:
	int32_t  sign;
	int32_t  exp;
	uint64_t frac;
};
