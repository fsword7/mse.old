/*
 * debug.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: fswor
 */


#include "emu/core.h"
#include "emu/debug.h"

Debug::Debug()
: flags(DBG_NONE)
{
}

Debug::~Debug()
{
}

void Debug::loadFlags(uint32_t flags)
{
	this->flags = flags;
}

void Debug::setFlags(uint32_t flags)
{
	this->flags |= flags;
}

void Debug::clearFlags(uint32_t flags)
{
	this->flags &= ~flags;
}

void Debug::open(std::string fname)
{
	logFile.open(fname);
}

void Debug::close()
{
	if (logFile.is_open())
		logFile.close();
}

void Debug::log(const char *fmt, ...)
{
	char    out[2048];
	va_list args;
	va_start(args, fmt);


	vsprintf(out, fmt, args);

	if (flags & DBG_CONSOLE)
		std::cout << out << std::endl;
	if ((flags & DBG_LOGFILE) && logFile.is_open())
		logFile << out << std::endl;

	// All done, release arguments.
	va_end(args);
}
