/*
 * emufile.cpp
 *
 *  Created on: Feb 25, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/emufile.h"


emuFile::emuFile(uint32_t openFlags)
: openFlags(openFlags),
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

	return osdFile::FAILURE;
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
