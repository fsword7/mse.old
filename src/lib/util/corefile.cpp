/*
 * fileio.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: fswor
 */

#include <iostream>

#include <cstdio>
#include <cstdint>
#include <cassert>

#include <zlib.h>

#include "osd/osdfile.h"
#include "lib/util/corefile.h"

osdFile::error coreFile::open(std::string &filename, uint32_t openFlags, ptr &file)
{
	osdFile::ptr	ofile;
	uint64_t		length = 0;

	auto const ferr = osdFile::open(filename, openFlags, ofile, length);
	if (ferr != osdFile::NONE)
		return ferr;

//	file = new cpreOSDFile(openFlags, ofile, length);

	return osdFile::NONE;
}
