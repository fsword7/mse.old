/*
 * debug.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: fswor
 */


#include "emu/core.h"
#include "emu/debug.h"
#include "emu/devsys.h"

logFile::logFile()
: logFlags(0)
{
}

logFile::~logFile()
{
	// Close all log files
	close(-1);
}

void logFile::open(const std::string fname, const uint32_t slot)
{
	if (slot < LOG_NFILES) {
		// Close old log file if opened
		if (outFile[slot].is_open())
			outFile[slot].close();

		// Open log file at specific slot
		outFile[slot].open(fname);
		logFlags |= (1u << slot);
	}
}

void logFile::close(const int32_t slot)
{
	if (slot < 0) {
		logFlags = 0;
		for (int idx = 0; idx < LOG_NFILES; idx++) {
			if (outFile[idx].is_open())
				outFile[idx].close();
		}
	} else if (slot < LOG_NFILES) {
		logFlags &= ~(1u << slot);
		if (outFile[slot].is_open())
			outFile[slot].close();
	}
}

void logFile::log(const uint32_t flags, const char *out)
{
	if ((logFlags & (flags & LOG_ALLFILES)) == 0)
		return;

	for (int idx = 0; idx < LOG_NFILES; idx++) {
		if (logFlags & (flags & (1u << idx)))
			outFile[idx] << out << std::endl;
	}
}

// ***********************************************************************
Debug::Debug()
: dbgFlags(DBG_NONE), logFlags(0), sdev(nullptr)
{
}

Debug::~Debug()
{
}

void Debug::loadFlags(uint32_t flags)
{
	dbgFlags = flags;
}

void Debug::setFlags(uint32_t flags)
{
	dbgFlags |= flags;
}

void Debug::clearFlags(uint32_t flags)
{
	dbgFlags &= ~flags;
}

void Debug::log(const char *fmt, ...)
{
	logFile *logFile;
	char     out[2048];
	va_list  args;

	va_start(args, fmt);

	vsprintf(out, fmt, args);

	if (logFlags & LOG_CONSOLE)
		std::cout << out << std::endl;
	if (sdev != nullptr) {
		logFile = sdev->getLogFile();
		logFile->log(logFlags, out);
	}

	// All done, release arguments.
	va_end(args);
}
