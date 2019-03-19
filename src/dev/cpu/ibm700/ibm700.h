/*
 * ibm700.h
 *
 *  Created on: Feb 26, 2017
 *      Author: Timothy Stark
 *
 *  Emulates IBM 701, 702, 704, 709, 7040, 7090, and 7094
 */

#pragma once

class i700_cpu_base : public cpu_device
{
public:
	i700_cpu_base() {};
	~i700_cpu_base() {};

	virtual void devStart() override;
	virtual void devStop() override;
	virtual void devReset() override;

private:
};
