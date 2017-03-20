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

	inline Device *getDevice() const { return cdev; }
	inline Device *getDialedDevice() const { return sdev; }

	void dialDevice(Device *dev)  { sdev = dev; }

private:
	Device *cdev; // Console device
	Device *sdev; // Selected device
};


