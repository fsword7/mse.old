/*
 * osdfile.cpp
 *
 *  Created on: Feb 26, 2019
 *      Author: Tim Stark
 */

#include <string>
#include <cassert>
#include <cerrno>

#include <fcntl.h>

#include "osd/osdfileio.h"


osdFile::~osdFile()
{
	close(fd);
}

osdFile::error osdError(int errCode)
{
	switch (errCode)
	{
	case 0:
		return osdFile::NONE;

	case ENOENT:
	case ENOTDIR:
		return osdFile::NOT_FOUND;

	case EACCES:
	case EROFS:
	case EEXIST:
	case EPERM:
	case EISDIR:
	case EINVAL:
		return osdFile::ACCESS_DENIED;

//	case ENFILE:
//	case EMFILE:
//		return osdFile::TOO_MANY_FILES;

	default:
		return osdFile::FAILURE;
	}
}

osdFile::error osdFile::open(const std::string &path, uint32_t openFlags, osdFile **file)
{
	int access;
	int fd = -1;
	int mode = 0666;

	assert(file != nullptr);

	// Determine POSIX access flags
	if (openFlags & OPEN_FLAG_WRITE) {
		access = (openFlags & OPEN_FLAG_READ) ? O_RDWR : O_WRONLY;
		access |= (openFlags & OPEN_FLAG_CREATE) ? (O_CREAT|O_TRUNC) : 0;
	} else if (openFlags & OPEN_FLAG_READ) {
		access = O_RDONLY;
	}
	access |= O_BINARY;

	fd = ::open(path.c_str(), access, mode);

	if (fd < 0)
		return osdError(errno);

	*file = new osdFile(fd);

	return osdFile::NONE;
}

osdFile::error osdFile::read(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual)
{
	ssize_t result;

	if (lseek(fd, offset, SEEK_SET) < 0)
		return osdError(errno);
	result = ::read(fd, buffer, size_t(count));
	if (result < 0)
		return osdError(errno);
	actual = result;
	return osdFile::NONE;
}

osdFile::error osdFile::write(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual)
{
	ssize_t result;

	if (lseek(fd, offset, SEEK_SET) < 0)
		return osdError(errno);
	result = ::write(fd, buffer, size_t(count));
	if (result < 0)
		return osdError(errno);
	actual = result;
	return osdFile::NONE;
}
