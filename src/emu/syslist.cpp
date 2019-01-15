/*
 * syslist.cpp
 *
 *  Created on: Jan 11, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "emu/driver.h"
#include "emu/syslist.h"

using namespace std;

const system_driver *system_list::find(string name)
{
	const system_driver *model;

	for (int idx = 0; syslist[idx]; idx++) {
		model = syslist[idx];
//		while (model->name != nullptr) {
//			if (string(model->name) == name)
//				return model;
//			model++;
//		}
		if (string(model->name) == name)
			return model;
	}
	return nullptr;
}

void system_list::list()
{
	const system_driver *model;

	for (int idx = 0; syslist[idx]; idx++) {
		model = syslist[idx];
//		while (model != nullptr) {
//			cout << "Model: " << model->name << endl;
//			model++;
//		}
		cout << "Model: " << model->name << endl;
	}
}
