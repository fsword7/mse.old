/*
 * fileio.h
 *
 *  Created on: Feb 25, 2019
 *      Author: fswor
 */

#pragma once

#include "osd/osdfile.h"

#define OPEN_FLAG_READ		0x0001	// open for read access
#define OPEN_FLAG_WRITE		0x0002	// open for write access
#define OPEN_FLAG_CREATE	0x0004	// create/truncate file

//class zlib_data
//{
//protected:
//	zlib_data(uint64_t offset)
//	: flgCompression(false), flgDecompression(false),
//	  realOffset(offset), nextOffset(offset)
//	{
//		// Initialize zlib stream
//		stream.zalloc = Z_NULL;
//		stream.zfree = Z_NULL;
//		stream.opaque = Z_NULL;
//		stream.avail_in = 0;
//		stream.avail_out = 0;
//	}
//
//public:
//
//	~zlib_data()
//	{
//		if (flgCompression)
//			deflateEnd(&stream);
//		else if (flgDecompression)
//			inflateEnd(&stream);
//	}
//
//	static int startCompression(uint64_t offset, zlib_data **data, int level)
//	{
//		zlib_data *zlib = new zlib_data(offset);
//
//		zlib->resetOutput();
//		auto const zerr = deflateInit(&zlib->stream, level);
//		if (zerr == Z_OK) {
//			zlib->flgCompression = true;
//			assert(data != nullptr);
//			*data = zlib;
//		}
//		return zerr;
//	}
//
//	static int startDecompression(uint64_t offset, zlib_data **data)
//	{
//		zlib_data *zlib = new zlib_data(offset);
//
//		auto const zerr = inflateInit(&zlib->stream);
//		if (zerr == Z_OK) {
//			zlib->flgDecompression = true;
//			assert(data != nullptr);
//			*data = zlib;
//		}
//		return zerr;
//	}
//
//	void resetInput()
//	{
//		stream.next_in  = buffer;
//		stream.avail_in = sizeof(buffer);
//	}
//
//	void resetOutput()
//	{
//		stream.next_out  = buffer;
//		stream.avail_out = sizeof(buffer);
//	}
//
//	int compress(bool finish = false)
//	{
//		assert(flgCompression);
//		return deflate(&stream, finish ? Z_FINISH : Z_NO_FLUSH);
//	}
//
//	int decompress()
//	{
//		assert(flgDecompression);
//		return inflate(&stream, Z_SYNC_FLUSH);
//	}
//
//private:
//	bool		flgCompression;
//	bool		flgDecompression;
//	uint64_t	realOffset, nextOffset;
//	z_stream	stream;
//	uint8_t		buffer[1024];
//};



// Handle files for disk or memory access
class coreFile
{
public:
	typedef coreFile *ptr;

	virtual ~coreFile();

	static osdFile::error open(std::string &filename, uint32_t openFlags, ptr &file);

	virtual int seek(int64_t offset, int whence) = 0;
	virtual uint64_t tell() const = 0;
	virtual uint64_t size() const = 0;

	virtual uint32_t read(void *buffer, uint32_t length) = 0;
	virtual uint32_t write(void *buffer, uint32_t length) = 0;

private:

};
