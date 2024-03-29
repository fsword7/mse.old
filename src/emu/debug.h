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
#define DBG_TRACE		0x80000000 // Tracing execution
#define DBG_OPERAND 	0x40000000 // Operand activity
#define DBG_IOREGS		0x20000000 // I/O register activity
#define DBG_IODATA		0x10000000 // I/O data activity
#define DBG_EXCEPTION	0x08000000 // Interrupt/Exception activity

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

// Enable/disable log slots
#define LOG_CTYSLOT		-1
#define LOG_ALLSLOTS	-2

class sysDevice;

struct dbgOption
{
	const std::string dbgName; // Debug/log name
	uint32_t          dbgFlag; // Specific flag(s)
};

class logFile
{
public:
	logFile();
	~logFile();

	void open(const std::string fname, const uint32_t slot);
	void close(const int32_t slot);

	void log(const uint32_t flags, const char *out);

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

//	int enableFlag(const std::string &name);
//	int disableFlag(const std::string &name);
//	int enableLog(const int slot);
//	int disableLog(const int slot);

	inline bool checkFlags(uint32_t flags) { return (dbgFlags & flags) == flags; }

//	void setDevice(device_t *dev) { sdev = dev; }

	void log(const char *fmt, ...);

private:
//	device_t    *sdev;
	uint32_t     dbgFlags;
	uint32_t     logFlags;
};
