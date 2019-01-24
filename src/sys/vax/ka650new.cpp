/*
 * ka650new.cpp - KA650 system device
 *
 *  Created on: Jan 9, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/devsys2.h"
#include "emu/driver.h"
#include "sys/vax/ka650new.h"

using namespace std;

// Create system routines
void ka650_device::mv3900()
{
	cout << "This is MicroVAX 3900 system" << endl;
}

void ka650_device::mv3900x()
{
	cout << "This is MicroVAX 3800x system" << endl;
}

// Initialize system routines
void ka650_device::mv3900_init()
{

}

void ka650_device::mv3900x_init()
{

}
