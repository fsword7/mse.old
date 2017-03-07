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
	uint18_t id, len;

	// Process EXE/SAV header
	do {
		id  = uint18_t(d36 >> H10_WIDTH) & H10_MASK;
		len = uint18_t(d36) & H10_MASK;

		switch (id) {
		case SAV_ID_DIRECTORY:
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

}
