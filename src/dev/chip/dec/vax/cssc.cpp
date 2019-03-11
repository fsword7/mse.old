/*
 * cssc.cpp - CVAX System Support Chip
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cssc.h"

DEFINE_DEVICE_TYPE(CSSC, cssc_device, "CSSC", "CVAX System Support Chip")

cssc_device::cssc_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CSSC, tag, owner, clock)
{
}

cssc_device::~cssc_device()
{
}

uint32_t cssc_device::read(offs_t offset, int acc)
{
	uint32_t data;

	switch (offset) {
	case SSC_nTIR0:
		data = 0;
		break;
	case SSC_nTIR1:
		data = 0;
		break;
	default:
		data = sscReg[offset];
		break;
	}

}

void cssc_device::write(offs_t offset, uint32_t data, int acc)
{
	uint32 ndata;

	if (acc < LN_LONG) {
		uint32_t sc = (offset & ALIGN_LONG) << 3;
		uint32_t mask = ((acc == LN_WORD) ? W_MASK : B_MASK) << sc;
		ndata = ((data << sc) & mask) | (sscReg[offset] & ~mask);
	} else
		ndata = data;

	switch (offset) {
	case SSC_nBASE:
		sscReg[offset] = (ndata & BASE_ADDR) | BASE_MBO;
		break;

	case SSC_nCR:
		sscReg[offset] &= (ndata & CNF_W1C);
		sscReg[offset] = (ndata & CNF_RW) | (sscReg[offset] & ~CNF_RW);
		break;

	case SSC_nBTO:
		sscReg[offset] &= (ndata & BTO_W1C);
		sscReg[offset] = (ndata & BTO_RW) | (sscReg[offset] & ~BTO_RW);
		break;

	case SSC_nDLEDR:
		sscReg[offset] = ndata & LED_MASK;
		break;

	case SSC_nTCR0:
		break;
	case SSC_nTNIR0:
		sscReg[offset] = ndata;
		break;
	case SSC_nTIVR0:
		sscReg[offset] = ndata & TIVR_VEC;
		break;

	case SSC_nTCR1:
		break;
	case SSC_nTNIR1:
		sscReg[offset] = ndata;
		break;
	case SSC_nTIVR1:
		sscReg[offset] = ndata & TIVR_VEC;
		break;

	case SSC_nAD0MAT:
	case SSC_nAD0MSK:
	case SSC_nAD1MAT:
	case SSC_nAD1MSK:
		sscReg[offset] = ndata & ADS_MASK;
		break;
	}
}
