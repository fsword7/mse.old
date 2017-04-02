/*
 * debug.h
 *
 *  Created on: Mar 28, 2017
 *      Author: fswor
 */

#pragma once

#define DBG_NONE    0x00000000 // All log flags off
#define DBG_ALL     0xFFFFFFFF // All log flags
#define DBG_CONSOLE 0x80000000 // Console output
#define DBG_LOGFILE 0x40000000 // File output

#define DBG_TRACE   0x80000000 // Tracing disassembly

class Debug
{
public:
	Debug();
	~Debug();

	// Update debug flags for logging purposes
	void loadFlags(uint32_t flags);
	void setFlags(uint32_t flags);
	void clearFlags(uint32_t flags);

	void open(std::string fname);
	void close();

	void log(const char *fmt, ...);

private:
	uint32_t      flags;
	std::ofstream logFile;
};
