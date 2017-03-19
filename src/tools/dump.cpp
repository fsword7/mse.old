/*
 * dump.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: Timothy Stark
 *
 *  dump utility
 *
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdint>

void printDump(uint32_t sAddr, uint8_t *data, uint32_t size)
{
	uint32_t addr;
	char    *ptr, *cptr;
	uint8_t  chr;
	int      idx;
	char     buf[100], cbuf[18];

	for (addr = 0; (addr < size);) {
		ptr  = buf;
		cptr = cbuf;
		ptr += sprintf(ptr, "%08X: ", sAddr + addr);
		for (idx = 0; (idx < 16) && (addr < size); addr++, idx++) {
			chr  = *data++;
			ptr += sprintf(ptr, "%02X%c", chr, (idx == 7) ? '-' : ' ');
			*cptr++ = isprint(chr) ? chr : '.';
		}
		for (; idx < 16; idx++) {
			ptr += sprintf(ptr, "   ");
			*cptr++ = ' ';
		}
		*ptr  = '\0';
		*cptr = '\0';

		printf("%s |%s|\n", buf, cbuf);
	}
}

int main(int argc, char **argv)
{
	std::fstream in;
	std::string  fname;
	uint8_t      data[512];
	uint32_t     addr = 0;

	if (argc < 2) {
		printf("Usage: dump [options] <file>\n");
		return 1;
	}

	fname = argv[1];
	in.open(fname, std::ios::in | std::ios::binary);

	while (in.good()) {
		in.read((char*)data, sizeof(data));

		printDump(addr, data, in.gcount());
		addr += in.gcount();
	}

	in.close();
	return 0;
}


