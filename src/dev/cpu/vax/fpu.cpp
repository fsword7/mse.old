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


sfp_t::sfp_t(uint32_t fp)
{
	unpack(fp);
}

sfp_t::~sfp_t()
{
}

int sfp_t::unpack(uint32_t fp)
{
	sign = fp & SFP_SIGN;
	exp  = SFP_GETEXP(fp);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = ZXTQ(SFP_GETFRAC(fp)) << (SFP_GUARD + 32);
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

	if (exp > SFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		*fp = 0;
		return VFP_UNFL;
	}

	tfp = frac >> (SFP_GUARD + 32);
	*fp = sign | SFP_SETEXP(exp) | SFP_SETFRAC(tfp);
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
		exp  = SFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = SFP_BIAS + 32;
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
	sign = fp[0] & DFP_SIGN;
	exp  = DFP_GETEXP(fp[0]);
	if (exp == 0) {
		if (sign & FP_SIGN)
			return VFP_FAULT;
		frac = 0;
	} else {
		frac = (ZXTQ(DFP_GETFRAC(fp[0])) << 32) | (FP_SWAP(fp[1]) << DFP_GUARD);
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

	if (exp > DFP_M_EXP) {
		return VFP_OVFL;
	} else if (exp <= 0) {
		fp[0] = 0;
		fp[1] = 0;
		return VFP_UNFL;
	}

	tfp   = frac >> DFP_GUARD;
	fp[0] = sign | DFP_SETEXP(exp) | DFP_SETFRAC(tfp >> 32);
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
		exp  = DFP_BIAS + 32;
		frac = ZXTQ(-val) << (FP_P_NORM - 31);
	} else {
		sign = 0;
		exp  = DFP_BIAS + 32;
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
