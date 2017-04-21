/*
 * debug.h
 *
 *  Created on: Mar 28, 2017
 *      Author: fswor
 */

#pragma once

#define DBG_NONE		0x00000000 // All log flags off
#define DBG_ALL			0xFFFFFFFF // All log flags

// Debug flags
#define DBG_TRACE		0x80000000 // Tracing disassembly

#define LOG_NFILES		8

// Log flags
#define LOG_CONSOLE		0x80000000 // Console output
#define LOG_ALLFILES	0x000000FF
#define LOG_FILE8		0x00000080
#define LOG_FILE7		0x00000040
#define LOG_FILE6		0x00000020
#define LOG_FILE5		0x00000010
#define LOG_FILE4		0x00000008
#define LOG_FILE3		0x00000004
#define LOG_FILE2		0x00000002
#define LOG_FILE1		0x00000001

class sysDevice;

class logFile
{
public:
	logFile();
	~logFile();

	void open(uint32_t nlog, std::string fname);
	void close(int32_t nlog);

	void log(uint32_t flags, const char *out);

private:
	uint32_t      logFlags;
	std::ofstream outFile[LOG_NFILES];
};

class Debug
{
public:
	Debug();
	~Debug();

	// Update debug flags for logging purposes
	void loadFlags(uint32_t flags);
	void setFlags(uint32_t flags);
	void clearFlags(uint32_t flags);

	inline bool checkFlags(uint32_t flags) { return (dbgFlags & flags) == flags; }

	void setDevice(sysDevice *dev) { sdev = dev; }

	void log(const char *fmt, ...);

private:
	sysDevice    *sdev;
	uint32_t      dbgFlags;
	uint32_t      logFlags;
};
