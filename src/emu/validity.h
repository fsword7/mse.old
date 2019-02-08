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

	void validate();

};
