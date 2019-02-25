/*
 * emufile.h
 *
 *  Created on: Feb 25, 2019
 *      Author: fswor
 */

#pragma once

#include "osd/osdfile.h"
#include "lib/util/unzip.h"

class emuFile
{
public:
	emuFile(uint32_t openFlags);
	virtual ~emuFile();

	// open/close function calls
	osdFile::error open(const std::string &name);
	osdFile::error openFile();
	void close();

protected:
	std::string fileName;		// base filename
	std::string pathName;		// full filename

	uint32_t	openFlags;		// Open flags for access

	osdFile		*file;

	// archive file parameters
	archive_file			*zipFile;
	std::vector<uint8_t>	zipData;
	uint64_t				zipLength;
};
