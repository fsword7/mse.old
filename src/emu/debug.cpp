/*
 * debug.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: fswor
 */


#include "emu/emucore.h"
#include "emu/commands.h"
#include "emu/debug.h"

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
			if (outFile[idx].is_open()) {
				outFile[idx].flush();
				outFile[idx].close();
			}
		}
	} else if (slot < LOG_NFILES) {
		logFlags &= ~(1u << slot);
		if (outFile[slot].is_open()) {
			outFile[slot].flush();
			outFile[slot].close();
		}
	}
}

void logFile::log(const uint32_t flags, const char *out)
{
	if ((logFlags & (flags & LOG_ALLFILES)) == 0)
		return;

	for (int idx = 0; idx < LOG_NFILES; idx++) {
		if (logFlags & (flags & (1u << idx))) {
			outFile[idx] << out;
			outFile[idx].flush();
		}
	}
}

// ***********************************************************************

static const dbgOption dbgList[7] =
{
		{ "trace",		DBG_TRACE },
		{ "operand",	DBG_OPERAND },
		{ "exception",	DBG_EXCEPTION },
		{ "interrupt",	DBG_EXCEPTION },
		{ "ioregs",		DBG_IOREGS },
		{ "iodata",		DBG_IODATA },
		{ "all",		DBG_ALL }
};

Debug::Debug()
: dbgFlags(DBG_NONE), logFlags(0) /* , sdev(nullptr) */
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

//cmdStatus Debug::enableFlag(const std::string &name)
//{
//	for (auto &&dbg : dbgList)
//		if (boost::iequals(name, dbg.dbgName)) {
//			dbgFlags |= dbg.dbgFlag;
//			return cmdStatus::cmdOk;
//		}
//	return cmdStatus::cmdInvalid;
//}
//
//cmdStatus Debug::disableFlag(const std::string &name)
//{
//	for (auto &&dbg : dbgList)
//		if (boost::iequals(name, dbg.dbgName)) {
//			dbgFlags &= ~dbg.dbgFlag;
//			return cmdStatus::cmdOk;
//		}
//	return cmdStatus::cmdInvalid;
//}
//
//int Debug::enableLog(const int slot)
//{
//	if (slot >= 0 && slot < LOG_NFILES)
//		logFlags |= (1u << slot);
//	else if (slot == LOG_CTYSLOT)
//		logFlags |= LOG_CONSOLE;
//	else if (slot == LOG_ALLSLOTS) {
//		for (int idx = 0; idx < LOG_NFILES; idx++)
//			logFlags |= (1u << idx);
//	}
//	else
//		return CMD_ERROR;
//	return CMD_OK;
//}
//
//int Debug::disableLog(const int slot)
//{
//	if (slot >= 0 && slot < LOG_NFILES)
//		logFlags &= ~(1u << slot);
//	else if (slot == LOG_CTYSLOT)
//		logFlags &= ~LOG_CONSOLE;
//	else if (slot == LOG_ALLSLOTS) {
//		for (int idx = 0; idx < LOG_NFILES; idx++)
//			logFlags &= ~(1u << idx);
//	} else
//		return CMD_ERROR;
//	return CMD_OK;
//}

void Debug::log(const char *fmt, ...)
{
	logFile *logFile;
	char     out[2048];
	va_list  args;

	va_start(args, fmt);

	vsprintf(out, fmt, args);

	if (logFlags & LOG_CONSOLE)
		std::cout << out;
//	if (sdev != nullptr) {
//		logFile = sdev->getLogFile();
//		logFile->log(logFlags, out);
//	}

	// All done, release arguments.
	va_end(args);
}
