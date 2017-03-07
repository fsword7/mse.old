//
//  pdp10.cpp
//  mse
//
//  Created by Timothy Stark on 10/28/16.
//

#include "emu/core.h"
#include "emu/devsys.h"
#include "system/drivers/pdp10.h"

GROUP(pdp10)
SYSTEM(dec2020, pdp10,   "DECsystem2020")

Device pdp10_sysDriver {
	"PDP10",
	"DECsystem-10",
	__FILE__,
};

// *************************************************************************

pdp10_sysDevice::pdp10_sysDevice()
: mem(nullptr)
{
}

pdp10_sysDevice::~pdp10_sysDevice()
{
	if (mem != nullptr)
		delete mem;
}

void pdp10_sysDevice::load(std::string fname)
{
	uint36_t d36; // 36-bit data buffer
	uint18_t idx1, idx2;

	// header block registers
	uint18_t id, len;
	uint18_t ndir;
	uint36_t dir[128];
	uint8_t  cbuf[SAV_BLK_SIZE*5];

	// data block registers
	uint18_t flags, fpage, ppage, count;
	uint32_t paddr;

	std::ifstream in(fname, std::ios::binary);

	// Process EXE/SAV header
	do {
		in.read((char*)cbuf, 5);
		id  = uint18_t(d36 >> H10_WIDTH) & H10_MASK;
		len = uint18_t(d36) & H10_MASK;

		switch (id) {
		case SAV_ID_DIRECTORY:
			ndir = len;
			if (ndir >= sizeof(dir)) {
				std::cerr << fname << ": Too long directory entry block" << std::endl;
				return;
			}
			in.read((char *)cbuf, ndir*5);
			for (idx1 = 0; idx1 < ndir; idx1++) {
				dir[idx1] = d36;
			}
			break;
		case SAV_ID_ENTRY_VECTOR:
			break;
		case SAV_ID_END_BLOCK:
			// All done, start data process
			std::cout << "End of header block" << std::endl;
			break;
		default:
			std::cout << "Unknown ID code: " << id << std::endl;
			// Skip data block
			break;
		}
	} while (id != SAV_ID_END_BLOCK);

	// Process data blocks
	for (idx1 = 0; idx1 < ndir; idx1 += 2) {
		flags = uint18_t(dir[idx1] >> H10_WIDTH) & H10_MASK;
		fpage = uint18_t(dir[idx1]) & H10_MASK;
		count = uint18_t(dir[idx1+1] >> H10_WIDTH) & H10_MASK;
		ppage = uint18_t(dir[idx1+1]) & H10_MASK;
		paddr = ppage << 9;

		while (count >= 0) {
			if (fpage) {
				// Loading data block
				in.read((char *)cbuf, SAV_BLK_SIZE*5);
				for (idx2 = 0; idx2 < SAV_BLK_SIZE; idx2++) {
					mem[paddr++] = d36;
				}
				fpage++;
			} else {
				// Clear data block
				for (idx2 = 0; idx2 < SAV_BLK_SIZE; idx2++)
					mem[paddr++] = 0;
			}

			ppage++;
			count -= SAV_BLK_SIZE;
		}
	}

	in.close();
}
