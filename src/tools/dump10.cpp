/*
 * dump10.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: Timothy Stark
 */

#include <iostream>
#include <fstream>
#include <cstdint>
#include <stdio.h>
#include <getopt.h>
#include "formats/dec/word10.h"
//#include "formats/dec/imge.h"

//    Core-Dump Format             ANSI Format
//
// 35 34 33 32 31 30 29 28   __ 35 34 33 32 31 20 29
// 27 26 25 24 23 22 21 20   __ 28 27 26 25 24 23 22
// 19 18 17 16 15 14 13 12   __ 21 20 19 18 17 16 15
// 11 10 09 08 07 06 05 04   __ 14 13 12 11 10 09 08
// __ __ __ __ 03 02 01 00   00 07 06 05 04 03 02 01


void printDump10(uint32_t &addr, uint8_t *data, uint32_t size)
{
	uint36_t data36;

	for (uint32_t idx = 0; idx < size; idx +=  5) {
		// ANSI format
		data36  = (uint36_t)(data[idx+0] & 0x7F) << 29;
		data36 |= (uint36_t)(data[idx+1] & 0x7F) << 22;
		data36 |= (uint36_t)(data[idx+2] & 0x7F) << 15;
		data36 |= (uint36_t)(data[idx+3] & 0x7F) << 8;
		data36 |= (uint36_t)(data[idx+4] & 0x7F) << 1;
		if (data[idx+4] & 0x80)
			data36 |= 1;

		printf("%06o: %06llo,,%06llo \n", addr, W10_LEFT(data36), W10_RIGHT(data36));
		addr++;
	}
}

int main(int argc, char **argv)
{
	std::fstream in;
	std::string  fname;
	uint8_t      data[500];
	uint32_t     addr = 0;

	if (argc < 2) {
		printf("Usage: dump10 [options] <file>\n");
		return 1;
	}

	fname = argv[1];
	in.open(fname, std::ios::in | std::ios::binary);

	while (in.good()) {
		in.read((char*)data, sizeof(data));

		printDump10(addr, data, in.gcount());
	}

	in.close();
}
