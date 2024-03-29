/*
 * emufile.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/emufile.h"

pathIterator::pathIterator(const std::string &searchPath)
: searchPath(searchPath),
  current(searchPath.cbegin()),
  isFirst(true)
{
}

//pathIterator::pathIterator(std::string &&searchPath)
//: searchPath(std::move(searchPath)),
//  current(searchPath.cbegin()),
//  isFirst(true)
//{
//}

void pathIterator::reset()
{
	current = searchPath.cbegin();
	isFirst = true;
}

bool pathIterator::next(std::string &pathName, const char *name)
{
	if (!isFirst && (searchPath.cend() == current))
		return false;

	auto const separate(std::find(current, searchPath.cend(), ';'));
	pathName.assign(current, separate);
	current = separate;
	if (searchPath.cend() != current)
		++current;

	if (name != nullptr) {
		if (!pathName.empty())
			pathName.append("/");
		pathName.append(name);
	}

	isFirst = false;
	return true;
}

// *************************************************************
emuFile::emuFile(uint32_t openFlags)
: openFlags(openFlags),
  iterator(std::string()),
  mediaPaths(std::string()),
  zipFile(nullptr),
  zipLength(0),
  file(nullptr)
{
}

//emuFile::emuFile(std::string &&searchPath, uint32_t openFlags)
//: openFlags(openFlags),
//  iterator(searchPath),
//  mediaPaths(std::move(searchPath)),
//  zipFile(nullptr),
//  zipLength(0),
//  file(nullptr)
//{
//}

emuFile::emuFile(const std::string &searchPath, uint32_t openFlags)
: openFlags(openFlags),
  iterator(searchPath),
  mediaPaths(std::move(searchPath)),
  zipFile(nullptr),
  zipLength(0),
  file(nullptr)
{
}

emuFile::~emuFile()
{
	close();
}


osdFile::error emuFile::open(const std::string &name)
{
	osdFile::error ferr;

	fileName = name;

	return openFile();
}

osdFile::error emuFile::openFile()
{
	if (file != nullptr)
		close();

	osdFile::error ferr = osdFile::NOT_FOUND;
//	while (iterator.next(pathName, fileName.c_str())) {
//		// Attempt to open that file directly
//		std::cout << "Trying that: " << pathName << std::endl;
//
//	}
	pathName = "fw";
	if (!pathName.empty())
		pathName.append("/");
	pathName.append(fileName);
//	std::cout << "Trying that: " << pathName << std::endl;

	ferr = coreFile::open(pathName, openFlags, file);

	return ferr;
}

void emuFile::close()
{
	// Release now
	if (file != nullptr)
		delete file;
	file = nullptr;
}

int emuFile::seek(int64_t offset, int whence)
{
	if (file != nullptr)
		return file->seek(offset, whence);
	return 1;
}

uint32_t emuFile::read(void *buffer, uint32_t length)
{
	if (file != nullptr)
		return file->read(buffer, length);
	return 0;
}

uint32_t emuFile::write(void *buffer, uint32_t length)
{
	if (file != nullptr)
		return file->write(buffer, length);
	return 0;
}
