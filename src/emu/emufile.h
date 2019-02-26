/*
 * emufile.h
 *
 *  Created on: Feb 25, 2019
 *      Author: fswor
 */

#pragma once

#include "osd/osdfileio.h"
#include "lib/util/unzip.h"


class pathIterator
{
public:
	pathIterator(const std::string &searchPath);
//	pathIterator(std::string &&sarchPath);

	bool next(std::string &pathName, const char *name = nullptr);
	void reset();

private:
	std::string					searchPath;
	std::string::const_iterator	current;
	bool						isFirst;
};

class emuFile
{
public:
	emuFile(uint32_t openFlags);
//	emuFile(std::string &&searchPath, uint32_t openFlags);
	emuFile(const std::string &searchPath, uint32_t openFlags);
	virtual ~emuFile();

	// open/close function calls
	osdFile::error open(const std::string &name);
	osdFile::error openFile();
	void close();

	uint32_t read(void *buffer, uint32_t length);
	uint32_t write(void *buffer, uint32_t length);

protected:
	std::string fileName;		// base filename
	std::string pathName;		// full filename

	pathIterator mediaPaths;
	pathIterator iterator;

	uint32_t	openFlags;		// Open flags for access

	osdFile		*file;

	// archive file parameters
	archive_file			*zipFile;
	std::vector<uint8_t>	zipData;
	uint64_t				zipLength;
};
