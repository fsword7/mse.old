/*
 * cqbic.cpp - CVAX Qbus Interface Chip
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cqbic.h"

#ifdef DEBUG
static const char *bicNames[] = \
	{ "SCR", "DSER", "MEAR", "SEAR", "MBR" };
#endif /* DEBUG */

DEFINE_DEVICE_TYPE(CQBIC, cqbic_device, "CQBIC", "CVAX Qbus Interface Chip")

cqbic_device::cqbic_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CQBIC, tag, owner, clock)
{
}

cqbic_device::~cqbic_device()
{
}

uint32_t cqbic_device::read(offs_t offset, int acc)
{
	return cqReg[offset];
}

void cqbic_device::write(offs_t offset, uint32_t data, int acc)
{
	uint32_t ndata;

	if (acc < LN_LONG) {
		uint32_t sc = (offset & ALIGN_LONG) << 3;
		uint32_t mask = ((acc == LN_WORD) ? MSK_WORD : MSK_BYTE) << sc;
		ndata = ((data << sc) & mask) | (cqReg[offset] & ~mask);
	} else
		ndata = data;

	switch (offset) {
	case CQ_nSCR:
		cqReg[offset] = (ndata & SCR_RW) | (cqReg[offset] & ~SCR_RW) | SCR_PWROK;
		break;

	case CQ_nDSER:
		cqReg[offset] &= ~ndata;
		break;

	case CQ_nMEAR:
	case CQ_nSEAR:
		break;

	case CQ_nMBR:
		cqReg[offset] = ndata & MBR_ADDR;
		break;
	}
}
