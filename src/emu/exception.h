/*
 * exception.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <exception>

class mseFatalError : public std::exception
{
public:
	mseFatalError(const char *format, ...);
	mseFatalError(int code, const char *format, ...);

	const char *str() const { return message; }
	int exitCode() const { return code; }

private:
	char message[1024];
	int  code;
};
