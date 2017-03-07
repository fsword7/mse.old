/*
 * devcore.h
 *
 *  Created on: Mar 3, 2017
 *      Author: fswor
 */

class sysDevice;

class appCore
{
public:
	appCore();
	~appCore();

	void findDevice(std::string devName);

private:
	std::vector<sysDevice*> devList;
};
