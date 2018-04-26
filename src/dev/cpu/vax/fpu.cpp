/*
 * fpu.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: Tim Stark
 *
 *  VAX floating points
 *
 */

#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devcpu.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/fpu.h"

static const int64_t fpRound[] = { SFP_ROUND, DFP_ROUND, GFP_ROUND, HFP_ROUND };
static const int32_t fpBias[]  = { UFP_BIAS,  UFP_BIAS,  GFP_BIAS,  HFP_BIAS  };
static const int32_t fpMask[]  = { 0x7F, 0x7FFF, 0x7FFFFFFF };

vaxfp_t::vaxfp_t(int _type)
: type(_type)
{
	bias  = fpBias[type];
	round = fpRound[type];
	sign  = 0;
	exp   = 0;
	frac  = 0;
	st    = VFP_OK;
}

vaxfp_t::~vaxfp_t()
{
}

int vaxfp_t::unpack(uint32_t *val)
{
	switch(type) {
	case SFP_TYPE:
		return unpackf(val);
	case DFP_TYPE:
		return unpackd(val);
	case GFP_TYPE:
		return unpackg(val);
	}

	return VFP_FAULT;
}

int vaxfp_t::unpackf(uint32_t *val)
{
	sign = *val & UFP_SIGN;
	exp  = UFP_GETEXP(*val);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ZXTQ(UFP_GETFRAC(*val)) << (UFP_GUARD + 32);
	}

	return VFP_OK;
}

int vaxfp_t::unpackd(uint32_t *val)
{
	sign = val[0] & UFP_SIGN;
	exp  = UFP_GETEXP(val[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ((ZXTQ(UFP_GETFRAC(val[0])) << 32) | ZXTQ(FP_SWAP(val[1]))) << UFP_GUARD;
	}

	return VFP_OK;
}

int vaxfp_t::unpackg(uint32_t *val)
{
	sign = val[0] & UFP_SIGN;
	exp  = GFP_GETEXP(val[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ((ZXTQ(GFP_GETFRAC(val[0])) << 32) | ZXTQ(FP_SWAP(val[1]))) << GFP_GUARD;
	}

	return VFP_OK;
}


int vaxfp_t::packf(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		*dst = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += SFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		*dst = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp  = frac >> (UFP_GUARD + 32);
	*dst = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp);
	return VFP_OK;
}

int vaxfp_t::packd(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += DFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> UFP_GUARD;
	dst[0] = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

int vaxfp_t::packg(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += GFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > GFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> GFP_GUARD;
	dst[0] = sign | GFP_SETEXP(exp) | GFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

#if 0
int vaxfp_t::packh(uint32_t *dst)
{
	uint64_t tfp;

	// If fraction is zero, return as zero.
	if (frac == 0) {
		dst[0] = dst[1] = dst[2] = dst[3] = 0;
		return VFP_OK;
	}

	// Round and normalize value.
	frac += HFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	// Check exponential field for overflowing or underflowing.
	if (exp > HFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		dst[0] = dst[1] = dst[2] = dst[3] = 0;
		return VFP_UNFL;
	}

	// Now packing sign, exponential and fraction together.
	tfp    = frac >> HFP_GUARD;
	dst[0] = sign | HFP_SETEXP(exp) | HFP_SETFRAC(tfp >> 32);
	dst[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}
#endif

void vaxfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else {
		// Shift fraction to left-justified.
		while ((frac & FP_NORM) == 0) {
			frac <<= 1;
			exp--;
		}
	}
}

void vaxfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// Convert integer (B/W/L) to F/D/G float
int vaxfp_t::converti(int32_t val, uint32_t *res, int type)
{
	vaxfp_t fp(type);

	// When determine value as zero, do nothing and return with zero
	if (val == 0) {
		res[0] = 0;
		if (type == DFP_TYPE)
			res[1] = 0;
		return VFP_OK;
	}

	// Determine value as positive or negative
	if (val < 0) {
		// Negative value
		fp.sign = FP_SIGN;
		fp.bias = fpBias[type] + 32;
		fp.frac = uint64_t(-val) << (FP_P_NORM - 31);
	} else {
		// Positive value
		fp.sign = 0;
		fp.bias = fpBias[type] + 32;
		fp.frac = uint64_t(val) << (FP_P_NORM - 31);
	}
	fp.normalize();

	// All done, pack into FP words
	switch (type) {
	case SFP_TYPE:
		return fp.packf(res);
	case DFP_TYPE:
		return fp.packd(res);
	case GFP_TYPE:
		return fp.packg(res);
	}
	return VFP_ERROR;
}

int vaxfp_t::convertfg(uint32_t *val, uint32_t *res)
{
	vaxfp_t fp = vaxfp_t(SFP_TYPE);

	fp.unpackf(val);
	fp.exp  = fp.exp - UFP_BIAS + GFP_BIAS;
	fp.type = GFP_TYPE;
	return fp.packg(res);
}

int vaxfp_t::convertgf(uint32_t *val, uint32_t *res)
{
	vaxfp_t fp = vaxfp_t(GFP_TYPE);

	fp.unpackg(val);
	fp.exp  = fp.exp - GFP_BIAS + UFP_BIAS;
	fp.type = SFP_TYPE;
	return fp.packf(res);
}

int vaxfp_t::convertfd(uint32_t *val, uint32_t *res)
{
	vaxfp_t fp = vaxfp_t(SFP_TYPE);

	fp.unpackf(val);
	fp.type = DFP_TYPE;
	return fp.packd(res);
}

#if 0
// ****************************************************************************

sfp_t::sfp_t(uint32_t fp)
{
	unpack(fp);
}

sfp_t::~sfp_t()
{
}

int sfp_t::unpack(uint32_t fp)
{
	sign = fp & UFP_SIGN;
	exp  = UFP_GETEXP(fp);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ZXTQ(UFP_GETFRAC(fp)) << (UFP_GUARD + 32);
	}
	return VFP_OK;
}

int sfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		*fp = 0;
		return VFP_OK;
	}

	frac += SFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		*fp = 0;
		return VFP_UNFL;
	}

	tfp = frac >> (UFP_GUARD + 32);
	*fp = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp);
	return VFP_OK;
}

void sfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void sfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// ********************************************************************************

dfp_t::dfp_t(uint32_t *fp)
{
	unpack(fp);
}

dfp_t::~dfp_t()
{
}

int dfp_t::unpack(uint32_t *fp)
{
	sign = fp[0] & UFP_SIGN;
	exp  = UFP_GETEXP(fp[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = (ZXTQ(UFP_GETFRAC(fp[0])) << 32) | (FP_SWAP(fp[1]) << UFP_GUARD);
	}
	return VFP_OK;
}

int dfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_OK;
	}

	frac += DFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > UFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_UNFL;
	}

	tfp   = frac >> UFP_GUARD;
	fp[0] = sign | UFP_SETEXP(exp) | UFP_SETFRAC(tfp >> 32);
	fp[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

void dfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void dfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = UFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}

// ********************************************************************************

gfp_t::gfp_t(uint32_t *fp)
{
	unpack(fp);
}

gfp_t::~gfp_t()
{
}

int gfp_t::unpack(uint32_t *fp)
{
	sign = fp[0] & GFP_SIGN;
	exp  = GFP_GETEXP(fp[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = (ZXTQ(GFP_GETFRAC(fp[0])) << 32) | (FP_SWAP(fp[1]) << GFP_GUARD);
	}
	return VFP_OK;
}

int gfp_t::pack(uint32_t *fp)
{
	uint64_t tfp;

	if (frac == 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_OK;
	}

	frac += GFP_ROUND;
	if ((frac & FP_NORM) == 0) {
		frac >>= 1;
		exp++;
	}

	if (exp > GFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_UNFL;
	}

	tfp   = frac >> GFP_GUARD;
	fp[0] = sign | GFP_SETEXP(exp) | GFP_SETFRAC(tfp >> 32);
	fp[1] = FP_SWAP(ZXTL(tfp));
	return VFP_OK;
}

void gfp_t::normalize()
{
	if (frac == 0) {
		sign = 0;
		exp = 0;
	} else while ((frac & FP_NORM) == 0) {
		frac <<= 1;
		exp--;
	}
}

void gfp_t::convert(int32_t val)
{
	if (val == 0) {
		sign = 0;
		exp  = 0;
		frac = 0;
	} else if (val < 0) {
		sign = FP_SIGN;
		exp  = GFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = GFP_BIAS + 32;
		frac = ZXTQ(val) << (FP_P_NORM - 31);
	}
	normalize();
}
#endif
