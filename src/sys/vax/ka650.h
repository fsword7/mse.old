/*
 * ka650.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Timothy Stark
 */

#pragma once

// KA650/KA655 Memory Map
//
// 00000000 +-------------------------------------+
//          |            Main Memory              |
//          |- - - - - - - - - - - - - - - - - - -|
//          |         Up to 64 MB Memory          |
// 04000000 +-------------------------------------+
//          |             Reserved                |
// 10000000 +-------------------------------------+
//          |     Secondary Cache Diagnostic      |
// 14000000 +-------------------------------------+
//          |             Reserved                |
// 20000000 +-------------------------------------+
//          :                                     :
//
// 00000000 +-------------------------------------+
//          |           Main Memory               |
//          |- - - - - - - - - - - - - - - - - - -|
//          |        Up to 512 MB Memory          |
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
// System Memory Map
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


// 30-bit Physical Address Access

// Memory Space (up to 64MB main memory)
#define RAM_BASE	0
#define RAM_WIDTH	26
#define RAM_SIZE	(1u << RAM_WIDTH)
#define RAM_MASK	(RAM_SIZE - 1)
#define RAM_END		(RAM_BASE + RAM_SIZE)

// Memory Space (up to 512MB main memory)
#define RAMX_BASE	0
#define RAMX_WIDTH	29
#define RAMX_SIZE	(1u << RAMX_WIDTH)
#define RAMX_MASK	(RAMX_SIZE - 1)
#define RAMX_END	(RAMX_BASE + RAMX_SIZE)

// Firmware Space (up to 128K ROM space)
#define ROM_BASE	0x20040000
#define ROM_WIDTH	17
#define ROM_SIZE	(1u << ROM_WIDTH)
#define ROM_MASK	(ROM_SIZE - 1)
#define ROM_END		(ROM_BASE + (ROM_SIZE * 2))

// Non-Volatile RAM - 1024 bytes
#define NVR_BASE	(SSC_BASE + 0x400)
#define NVR_WIDTH	10
#define NVR_SIZE	(1u << NVR_WIDTH)
#define NVR_MASK	(NVR_SIZE - 1)
#define NVR_END		(NVR_BASE + NVR_SIZE)



// Local Register Area (20080000 to 20100000)
#define REG_BASE	0x20080000             // Local Register Base Address
#define REG_WIDTH	19                     //    Bus Width
#define REG_SIZE	(1u << REG_WIDTH)      //    Total Bytes
#define REG_END		(REG_BASE + REG_SIZE)  // End of Local Register Area



// CMCTL - Memory Controller Registers
#define CM_BASE     (REG_BASE + 0x100)     // CMCTL Base Address
//#define CM_NREGS    18                     //    Number of Registers
#define CM_NREGS    19                     //    Number of Registers
#define CM_SIZE     (CM_NREGS << 2)        //    Total Bytes
#define CM_END      (CM_BASE + CM_SIZE)    // End of CMCTL Registers
#define CM_REG(rn)  cmReg[rn]

#define CM_nERR     16 // CMCTL Error Register
#define CM_nCSR     17 // CMCTL Control/Status Register
#define CM_nMSZ     18 // CMCTL Memory Size

#define CM_ERR      CM_REG(CM_nERR)
#define CM_CSR      CM_REG(CM_nCSR)
#define CM_MSZ      CM_REG(CM_nMSZ)

// CMCTL Configuration Register
#define CMCNF_VLD   0x80000000  // Address Valid
#define CMCNF_BA    0x1FF00000  // Base Address
#define CMCNF_LOCK  0x00000040  // Lock
#define CMCNF_SRQ   0x00000020  // Signature Request
#define CMCNF_SIG   0x0000001F  // Signature

#define CMCNF_RW    (CMCNF_VLD|CMCNF_BA)
#define CMCNF_MASK  (CMCNF_RW|CMCNF_SIG)

#define MEM_BANK    (1u << 22) // Bank Size
#define MEM_SIG     0x17       // ECC, 4 x 4 MB

// CMCTL Error Register
#define CMERR_RDS  0x80000000 // Uncorrected Error
#define CMERR_FRQ  0x40000000 // 2nd RDS
#define CMERR_CRD  0x20000000 // CRD Error
#define CMERR_PAG  0x1FFFFC00 // Page Address
#define CMERR_DMA  0x00000100 // DMA Error
#define CMERR_BUS  0x00000080 // Bus Error
#define CMERR_SYN  0x0000007F // Syndrome

#define CMERR_W1C  (CMERR_RDS|CMERR_FRQ|CMERR_CRD|CMERR_DMA|CMERR_BUS)

// CMCTL Control/Status Register
#define CMCSR_PMI  0x00002000 // PMI Speed
#define CMCSR_CRD  0x00001000 // Enable CRD Interrupt
#define CMCSR_FRF  0x00000800 // Force Reference
#define CMCSR_DET  0x00000400 // Dis Error
#define CMCSR_FDT  0x00000200 // Fast Diagnostic
#define CMCSR_DCM  0x00000080 // Diagnostic Mode
#define CMCSR_SYN  0x0000007F // Syndrome

#define CMCSR_RW (CMCSR_PMI|CMCSR_CRD|CMCSR_DET|CMCSR_FDT|CMCSR_DCM|CMCSR_SYN)



// KA650 Board Registers
#define KA_BASE    (REG_BASE + 0x4000)    // KA650 Base Address
#define KA_NREGS   2                      //    Number of Registers
#define KA_SIZE    (KA_NREGS << 2)        //    Total Bytes
#define KA_END     (KA_BASE + KA_SIZE)    // End of KA650 Registers
#define KA_REG(rn) kaReg[rn]

#define KA_nCACR  0 // Cache Control Register
#define KA_nBDR   1

#define KA_CACR   KA_REG(KA_nCACR)
#define KA_BDR    KA_REG(KA_nBDR)



// KA650 Cache Control Register
#define CACR_DRO    0x00FFFF00 // Diagnostics Bits
#define CACR_FIXED  0x00000040 // Fixed Bits
#define CACR_CPE    0x00000020 // Parity Error
#define CACR_CEN    0x00000010 // Cache Enable
#define CACR_DPE    0x00000004 // Diable Parity
#define CACR_WWP    0x00000002 // Write Wrong Parity
#define CACR_DIAG   0x00000001 // Diagnostic Mode
#define CACR_P_DPAR 24

#define CACR_W1C    (CACR_CPE)
#define CACR_RW     (CACR_CEN|CACR_DPE|CACR_WWP|CACR_DIAG)



// SSC Registers for MicroVAX III series
#define SSC_BASE    0x20140000             // SSC Base Address
#define SSC_NREGS   84                     //    Number of Registers
#define SSC_SIZE    (SSC_NREGS << 2)       //    Total Bytes
#define SSC_END     (SSC_BASE + SSC_SIZE)  // End of SSC Registers
#define SSC_REG(rn) ssReg[rn]

#define SSC_nBASE   0x00 // Base Address
#define SSC_nCR     0x04 // Configuration Register
#define SSC_nBTO    0x08 // CDAL
#define SSC_nDLEDR  0x0C // Display LED Register
#define SSC_nTODR   0x1B // Time of Day Register
#define SSC_nCSRS   0x1C // Console Storage Receive Status
#define SSC_nCSRD   0x1D // Console Storage Receive Data
#define SSC_nCSTS   0x1E // Console Storage Transmit Status
#define SSC_nCSTD   0x1F // Console Storage Transmit Data
#define SSC_nRXCS   0x20 // Receive Control/Status Register
#define SSC_nRXDB   0x21 // Receive Data Buffer
#define SSC_nTXCS   0x22 // Transmit Control/Status Register
#define SSC_nTXDB   0x23 // Transmit Data Buffer
#define SSC_nTCR0   0x40 // Timer 0 Configuration Register
#define SSC_nTIR0   0x41 // Timer 0 Interval Register
#define SSC_nTNIR0  0x42 // Timer 0 Next Interval Register
#define SSC_nTIVR0  0x43 // Timer 0 Interrupt Vector Register
#define SSC_nTCR1   0x44 // Timer 1 Configuration Register
#define SSC_nTIR1   0x45 // Timer 1 Interval Register
#define SSC_nTNIR1  0x46 // Timer 1 Next Interval Register
#define SSC_nTIVR1  0x47 // Timer 1 Interrupt Vector Register
#define SSC_nAD0MAT 0x4C // Address Strobe 0 Match Register
#define SSC_nAD0MSK 0x4D // Address Strobe 0 Mask Register
#define SSC_nAD1MAT 0x50 // Address Strobe 1 Match Register
#define SSC_nAD1MSK 0x51 // Address Strobe 1 Mask Register

#define SSC_xBASE  SSC_REG(SSC_nBASE)   // SSC Base Address
#define SSC_CR     SSC_REG(SSC_nCR)     // SSC Configuration Register
#define SSC_BTO    SSC_REG(SSC_nBTO)    // SSC Bus Timeout Register
#define SSC_LED    SSC_REG(SSC_nDLEDR)  // SSC Display LED Register
#define SSC_TCR0   SSC_REG(SSC_nTCR0)   // SSC Timer #0 - Config Register
#define SSC_TIR0   SSC_REG(SSC_nTIR0)   // SSC Timer #0 - Interval Register
#define SSC_TNIR0  SSC_REG(SSC_nTNIR0)  // SSC Timer #0 - Next Interval Reg
#define SSC_TIVR0  SSC_REG(SSC_nTIVR0)  // SSC Timer #0 - Int Vec Register
#define SSC_TCR1   SSC_REG(SSC_nTCR1)   // SSC Timer #1 - Config Register
#define SSC_TIR1   SSC_REG(SSC_nTIR1)   // SSC Timer #1 - Interval Register
#define SSC_TNIR1  SSC_REG(SSC_nTNIR1)  // SSC Timer #1 - Next Interval Reg
#define SSC_TIVR1  SSC_REG(SSC_nTIVR1)  // SSC Timer #1 - Int Vec Register
#define SSC_AD0MAT SSC_REG(SSC_nAD0MAT) // SSC Address Strobe #0 Match
#define SSC_AD0MSK SSC_REG(SSC_nAD0MSK) // SSC Address Strobe #0 Mask
#define SSC_AD1MAT SSC_REG(SSC_nAD1MAT) // SSC Address Strobe #1 Match
#define SSC_AD1MSK SSC_REG(SSC_nAD1MSK) // SSC Address Strobe #1 Mask

#define SSC_TCR(tmr)  SSC_REG(SSC_nTCR0 + ((tmr) * SSC_TMR))
#define SSC_TIR(tmr)  SSC_REG(SSC_nTIR0 + ((tmr) * SSC_TMR))
#define SSC_TNIR(tmr) SSC_REG(SSC_nTNIR0 + ((tmr) * SSC_TMR))
#define SSC_TIVR(tmr) SSC_REG(SSC_nTIVR0 + ((tmr) * SSC_TMR))

// SSC - Base Register
#define BASE_ADDR  0x1FFFFC00 // SSC Base Address
#define BASE_MBO   0x20000000 //   Must Be One

// SSC - Configuration Register
#define CNF_BLO    0x80000000 // Low Battery
#define CNF_IVD    0x08000000
#define CNF_IPL    0x03000000
#define CNF_ROM    0x00F70000 // ROM Parameters
#define CNF_CTLP   0x00008000 // CTRL-P Enable
#define CNF_BAUD   0x00007700 // Baud Rates
#define CNF_ADR    0x00000077 // Address

#define CNF_W1C    (CNF_BLO)
#define CNF_RW     0x0BF7F777

// SSC - Bus Timeout Register
#define BTO_BTO    0x80000000 // Bus Timeout
#define BTO_RWT    0x40000000 // Read/Write Access
#define BTO_INTV   0x00FFFFFF

#define BTO_W1C    (BTO_BTO|BTO_RWT)
#define BTO_RW     (BTO_INTV)

// SSC - Display LED Register
#define LED_MASK   0x0000000F

// SSC - Timer Registers
#define TCR_ERR    0x80000000 // Timer Error
#define TCR_DONE   0x00000080 // Done
#define TCR_IE     0x00000040 // Interrupt Enable
#define TCR_STEP   0x00000020 // Single Step
#define TCR_XFER   0x00000010 // Transfer
#define TCR_STOP   0x00000004 // Stop
#define TCR_RUN    0x00000001 // Run
#define TCR_W1C    (TCR_ERR|TCR_DONE)
#define TCR_RW     (TCR_IE|TCR_STOP|TCR_RUN)

#define TIVR_VEC   0x3FC      // Interrupt Vector Mask

#define TMR_NREGS  2      // Number of Registers
#define TMR0_VEC   0x078  // Timer #0 Vector Address
#define TMR1_VEC   0x07C  // Timer #1 Vector Address
#define TMR_IPL    14 // UQ_BR4 // Interrupt Priority Level
#define TMR_PRIO   25     // Device Priority
#define TMR_TICK   1000   // Each 1 kHz
#define SSC_TMR    4      // Timer Index
#define SSC_TMR0   0
#define SSC_TMR1   4

// SSC - Address Strobes
#define ADS_MASK   0x3FFFFFFC



// Cache Diagnostic Space
#define CDG_BASE   0x10000000
#define CDG_SIZE   (CDA_SIZE * CTA_SIZE)
#define CDG_END    (CDG_BASE + CDG_SIZE)

#define CDA_WIDTH  16
#define CDA_SIZE   (1u << CDA_WIDTH)
#define CDA_MASK   (CDA_SIZE - 1)

#define CTA_WIDTH  10
#define CTA_SIZE   (1u << CTA_WIDTH)
#define CTA_MASK   (CTA_SIZE - 1)

#define CTG_V      (1u << (CTA_WIDTH + 0)) // Valid Bit
#define CTG_WP     (1u << (CTA_WIDTH + 1)) // Wrong Parity Bit

#define CDG_GETROW(x)      (((x) & CDA_MASK) >> 2)
#define CDG_GETTAG(x)      (((x) >> CDA_WIDTH) & CTA_MASK)
#define CDG_DATA(reg)      cdData[reg]



#if 0
// Model-Specific Interrupt Priority Levels
#define IPL_CLKINT  0x18  // Clock IPL Interrupt
#define IPL_TTYINT  0x14  // Console IPL Interrupt

// Model-Specific IPR Registers
#define IPR_CONPC    CPU_IPR(PR_CONPC)
#define IPR_CONPSL   CPU_IPR(PR_CONPSL)
#define IPR_IORESET  CPU_IPR(PR_IORESET)
#define IPR_MSER     CPU_IPR(PR_MSER)
#define IPR_CADR     CPU_IPR(PR_CADR)

#endif



class ka650_sysDevice : public vax_sysDevice
{
public:
	ka650_sysDevice();
	~ka650_sysDevice();

	static ka650_sysDevice *create(std::string devName, std::string devType, sysModel *model);

	void reset();

	uint32_t readka(uint32_t pAddr, uint32_t acc);
	void     writeka(uint32_t pAddr, uint32_t data, uint32_t acc);
	uint32_t readcm(uint32_t pAddr, uint32_t acc);
	void     writecm(uint32_t pAddr, uint32_t data, uint32_t acc);
	uint32_t readssc(uint32_t pAddr, uint32_t acc);
	void     writessc(uint32_t pAddr, uint32_t data, uint32_t acc);
	uint32_t readcdg(uint32_t pAddr, uint32_t acc);
	void     writecdg(uint32_t pAddr, uint32_t data, uint32_t acc);

	uint32_t readio(uint32_t pAddr, uint32_t acc);
	void     writeio(uint32_t pAddr, uint32_t data, uint32_t acc);

private:
	// local I/O register area
	uint32_t ssReg[SSC_NREGS];
	uint32_t kaReg[KA_NREGS];
	uint32_t cmReg[CM_NREGS];

	// system timers
	uint32_t tmrClock[TMR_NREGS];
	uint64_t tmrLast[TMR_NREGS];

	// Cache diagnostic data area
	uint32_t cdData[CDA_SIZE >> 2];

	// SSC NVRAM data area
	uint8_t  nvrData[NVR_SIZE];

	// Firmware space
	uint8_t	*romData;
	uint32_t romSize;

	// Q22 Bus I/O space
};
