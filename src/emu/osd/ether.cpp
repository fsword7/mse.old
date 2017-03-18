/*
 * ether.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Timothy Stark
 *
 *  Virtual Ethernet interface
 *
 */

#include "emu/core.h"
#include "emu/osd/socket.h"

#if defined(HAVE_PCAP)
#define USE_BPF 1
#include <pcap.h>
#endif


