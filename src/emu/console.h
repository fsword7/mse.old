/*
 * console.h
 *
 *  Created on: Mar 2, 2017
 *      Author: fswor
 */

#pragma once


class Device;

class Console {
public:
	Console(Device *dev);
	~Console();

	void prompt();

	inline Device *getDevice() const { return root; }

private:
	Device *root; // root core device
};


