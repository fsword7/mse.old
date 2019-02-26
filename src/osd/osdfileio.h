/*
 * osdfile.h
 *
 *  Created on: Feb 25, 2019
 *      Author: Tim Stark
 */

#pragma once

#define OPEN_FLAG_READ		0x0001	// open for read access
#define OPEN_FLAG_WRITE		0x0002	// open for write access
#define OPEN_FLAG_CREATE	0x0004	// create/truncate file


class osdFile
{
public:
	enum error {
		NONE,
		FAILURE,
		OUT_OF_MEMORY,
		NOT_FOUND,
		ACCESS_DENIED,
		INVALID_DATA,
		INVALID_ACCESS
	};


	osdFile(int fd) : fd(fd) {}

	virtual ~osdFile();

	static error open(const std::string &path, uint32_t openFlags, osdFile **file);

protected:
	int fd;
};
