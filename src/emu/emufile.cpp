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
	std::cout << "Trying that: " << pathName << std::endl;

	ferr = osdFile::open(pathName, openFlags, &file);
	if (ferr == osdFile::NONE) {
		std::cout << "Successfully open file!" << std::endl;
		if (file != nullptr) {
			std::cout << "Now closed it for now." << std::endl;
			delete file;
			file = nullptr;
		}
	}

	return ferr;
}

void emuFile::close()
{
	// Release now
	file = nullptr;
}

emuFile::~emuFile()
{
	close();
}
