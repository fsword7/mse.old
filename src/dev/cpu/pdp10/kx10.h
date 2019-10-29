/*
 * kx10.h - PDP-10 family processor package
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 *
 *  Digital Equipment Corporation - 36-bit PDP-10 family systems
 *
 *  name	MIPS	Year	Descriptions
 *
 *  PDP-6	0.25	1964	Original 36-bit implementation
 *  KA10	0.38	1967	First PDP-10, flip chips
 *  KI10	0.72	1972	FIrst paging system, flip chips + MSI
 *  KL10A	1.8		1975	First ECL system, ECL 10K
 *  KL10B	1.8		1978	Extended addressing, ECL 10K
 *  KS10	0.3		1979	2901-based chips, last 36-bit system
 *
 *
 *	In additional, it ran one of major operating systems
 *
 *	TOPS-10		Original timesharing system
 *	TOPS-20		Commercial version of TENEX
 *	ITS			Incompatible timesharing system
 *	TENEX		ARPA-sponsored timesharing system
 *
 */

#pragma once

#include "emu/devcpu.h"
#include "formats/dec/word10.h"

#define AC_NBLKS      8
#define AC_NREGS    020

// Define only one of CPU types for compiling time
//#define CPU_KS10 1

// PDP6 processor
#ifndef CPU_PDP6
#define CPU_PDP6 0
#endif

// KA10 processor
#ifndef CPU_KA10
#define CPU_KA10 0
#endif

// KI10 processor
#ifndef CPU_KI10
#define CPU_KI10 0
#endif

// KL10A processor
#ifndef CPU_KL10A
#define CPU_KL10A 0
#endif

// KL10B processor with extended addressing
#ifndef CPU_KL10B
#define CPU_KL10B 0
#endif

// KL10 processor - either KL10A or KL10B
#ifndef CPU_KL10
#define CPU_KL10 (CPU_KL10A|CPU_KL10B)
#endif

// KS10 processor
#ifndef CPU_KS10
#define CPU_KS10 0
#endif

#if (CPU_PDP6 + CPU_KA10 + CPU_KI10 + CPU_KL10 + CPU_KS10) != 1
#error "Please define only one of CPU types - aborted"
#endif

#ifndef KI10_22BIT
#define KI10_22BIT (CPU_KI10|CPU_KL10)
#endif

// DEC operating systems (TOPS-10 and TOPS-20)
#ifndef OS_DEC
#define OS_DEC 0
#endif

// MIT Incomputable Timesharing System
#ifndef OS_ITS
#define OS_ITS 0
#endif

#if (OS_DEC + OS_ITS) != 1
#error "Please define only one of OS types - aborted"
#endif

// Determine maximum memory size for one of PDP-10 processors
#if KI10_22BIT|CPU_KI10
#define MEM_MAXSIZE (4096 * 1024)
#else
#if CPU_PDP6
#define MEM_MAXSIZE (256 * 1024)
#else
#define MEM_MAXSIZE (1024 * 1024)
#endif
#endif

// Determine address width for one of PDP-10 processors
#if KL10_22BIT
#define PADR_WIDTH	22
#define PADR_MASK	017777777LL
#define CTX_MASK	0037777LL
#else
#define PADR_WIDTH	18
#define PADR_MASK	W10_RMASK
#define CTX_MASK	W10_RMASK
#endif

// PC flags (left halfword)
#define PCF_AOV			0400000 // Arithmetic overflow
#define PCF_PCH			0400000 // PC changing (PDP6)
#define PCF_PCP			0400000 // Previous context public
#define PCF_FOV			0040000 // Floating overflow
#define PCF_CR0			0200000 // Carry #0
#define PCF_CR1			0100000 // Carry #1
#define PCF_FPD			0020000 // First part done
#define PCF_USR			0010000 // User mode
#define PCF_UIO			0004000 // User I/O mode
#define PCF_PCU			0004000 // Previous context user
#define PCF_PUB			0002000 // Public mode
#define PCF_AFI			0001000 // Address failure inhibit
#define PCF_1PR			0001000 // 1-Proceed flag (ITS)
#define PCF_TR2			0000400 // Trap #2 (Stack trap)
#define PCF_TR1			0000200 // Trap #1 (arithmetic trap)
#define PCF_FXU			0000100 // Floating underflow
#define PCF_DCX			0000040 // NO divide (divide check)

#define PCF_FLAGS		0777740 // PC flags field
#define PCF_SECTION		0000037 // PC section field
#define PCF_TRAPS		(PCF_TR1|PCF_TR2)


class pdp10_cpu_base : public cpu_device
{
public:
	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	// Accumulators
	w10_t  acRegs[AC_NBLKS][AC_NREGS];
	// Current/Previous Accumulators
	w10_t *curReg; // Current AC block
	w10_t *prvReg; // Previous AC block

	pdp10_cpu_base()  {}
	virtual ~pdp10_cpu_base() {}

	virtual void execute();
};
