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


class coreMemoryFile : public coreFile
{
public:
	coreMemoryFile(uint32_t openFlags, uint64_t fsize)
	: openFlags(openFlags)
	{
	}

	virtual ~coreMemoryFile()
	{
	}

	virtual int seek(int64_t offset, int whence)
	{
		return 0;
	}

	virtual uint64_t tell() const
	{
		return 0;
	}

	virtual uint64_t size() const
	{
		return 0;
	}

	virtual uint32_t read(void *buffer, uint32_t length)
	{
		return 0;
	}

	virtual uint32_t write(void *buffer, uint32_t length)
	{
		return 0;
	}

private:
	uint32_t		openFlags;
};

// ********************************************************************

class coreSystemFile : public coreFile
{
public:
	coreSystemFile(uint32_t openFlags, osdFile::ptr &&file, uint64_t fsize)
	: openFlags(openFlags),
	  file(std::move(file)),
	  base(0), bytes(0),
	  fileOffset(0),
	  fileSize(fsize)
	{
	}

	virtual ~coreSystemFile()
	{
		if (file != nullptr)
			delete file;
	}

	virtual int seek(int64_t offset, int whence)
	{
		switch (whence) {
		case SEEK_SET:
			fileOffset = offset;
			break;

		case SEEK_CUR:
			fileOffset += offset;
			break;

		case SEEK_END:
			fileOffset = fileSize + offset;
			break;
		}
		return 0;
	}

	virtual uint64_t tell() const
	{
		return 0;
	}

	virtual uint64_t size() const
	{
		return 0;
	}

	virtual uint32_t read(void *buffer, uint32_t length)
	{
		bytes = 0;

		uint32_t actual = 0;
		osdRead(buffer, offset(), length, actual);
		addOffset(actual);

		return actual;
	}

	virtual uint32_t write(void *buffer, uint32_t length)
	{
		bytes = 0;

		uint32_t actual = 0;
		osdWrite(buffer, offset(), length, actual);
		addOffset(actual);

		return actual;
	}

protected:

	uint64_t offset() { return fileOffset; }
	void addOffset(uint64_t increment)
	{
		fileOffset += increment;
		fileSize   = std::max(fileOffset, fileSize);
	}

	osdFile::error osdRead(void *buffer, uint64_t offset, uint32_t length, uint32_t &actual)
	{
		if (file != nullptr)
			return file->read(buffer, offset, length, actual);
		return osdFile::FAILURE;
	}

	osdFile::error osdWrite(void *buffer, uint64_t offset, uint32_t length, uint32_t &actual)
	{
		if (file != nullptr)
			return file->write(buffer, offset, length, actual);
		return osdFile::FAILURE;
	}


private:
	static constexpr size_t FILE_BUFFER_SIZE = 512;

	osdFile::ptr	file;
	uint32_t		openFlags;
	uint64_t		base;
	uint32_t		bytes;
	uint8_t			buffer[FILE_BUFFER_SIZE];

	uint64_t		fileOffset;
	uint64_t		fileSize;
};

// ********************************************************************

osdFile::error coreFile::open(std::string &filename, uint32_t openFlags, ptr &file)
{
	osdFile::ptr sysFile;
	uint64_t fsize = 0;

	auto const ferr = osdFile::open(filename, openFlags, sysFile, fsize);
	if (ferr != osdFile::NONE)
		return ferr;

	file = new coreSystemFile(openFlags, std::move(sysFile), fsize);

	return osdFile::NONE;
}
