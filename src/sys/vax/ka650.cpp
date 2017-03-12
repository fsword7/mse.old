/*
 * ka650.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  MicroVAX 3900 series system
 *
 */

// KA650/KA655/KA655X - CVAX Memory Map
//
// 00000000 +-------------------------------------+
//          |           Main Memory               |
//          |- - - - - - - - - - - - - - - - - - -|
//          |       Up to 64/512 MB Memory        |
// 01000000 +-------------------------------------+
//          |             Reserved                |
// 20000000 +-------------------------------------+
//          |           QBus I/O space            |
// 20002000 +-------------------------------------+
//          |             Reserved                |
// 20040000 +-------------------------------------+
//          |               ROM                   |
// 20080000 +-------------------------------------+
//          |      Local I/O Register Area        |
// 20200000 +-------------------------------------+
//          |             Reserved                |
// 30000000 +-------------------------------------+
//          |          QBus Memory Space          |
// 303FFFFF +-------------------------------------+
//          |             Reserved                |
// 3FFFFFFF +-------------------------------------+
//
// Systen Memory Map
//
//   0000 0000 - 03FF FFFF  KA650/KA655 Main Memory (Up to 64 MB)
//   1000 0000 - 13FF FFFF  KA650/KA655 Secondary Cache Diagnostic Space
//   0000 0000 - 1FFF FFFF  KA655X Main Memory (Up to 512 MB)
//
//   2000 0000 - 2000 1FFF  Qbus I/O Page
//   2004 0000 - 2005 FFFF  ROM Space, Halt Protected
//   2006 0000 - 2007 FFFF  ROM Space, Halt Unprotected
//   2008 0000 - 201F FFFF  Local Register Space
//   3000 0000 - 303F FFFF  Qbus Memory Space

