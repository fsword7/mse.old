/*
 * validity.h
 *
 *  Created on: Feb 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class validity_checker
{
public:
	validity_checker();
	~validity_checker();

	void checkDriver();

	void validate_system();
	void validate_devices();

	void begin();
	void end();
	void check(device_t *system);

	void validate();

private:
	int		errors;
	int		warnings;
	bool	validate_all;
};
