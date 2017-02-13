/*
 * opcode.h
 *
 *  Created on: Feb 12, 2017
 *      Author: fswor
 */

#pragma once

#define OPC_nUUO		0000	// Unimplemented Instruction
#define OPC_nLUUO01		0001
#define OPC_nLUUO02		0002
#define OPC_nLUUO03		0003
#define OPC_nLUUO04		0004
#define OPC_nLUUO05		0005
#define OPC_nLUUO06		0006
#define OPC_nLUUO07		0007

#define OPC_nLUUO10		0010
#define OPC_nLUUO11		0011
#define OPC_nLUUO12		0012
#define OPC_nLUUO13		0013
#define OPC_nLUUO14		0014
#define OPC_nLUUO15		0015
#define OPC_nLUUO16		0016
#define OPC_nLUUO17		0017

#define OPC_nLUUO20		0020
#define OPC_nLUUO21		0021
#define OPC_nLUUO22		0022
#define OPC_nLUUO23		0023
#define OPC_nLUUO24		0024
#define OPC_nLUUO25		0025
#define OPC_nLUUO26		0026
#define OPC_nLUUO27		0027

#define OPC_nLUUO30		0030
#define OPC_nLUUO31		0031
#define OPC_nLUUO32		0032
#define OPC_nLUUO33		0033
#define OPC_nLUUO34		0034
#define OPC_nLUUO35		0035
#define OPC_nLUUO36		0036
#define OPC_nLUUO37		0037


#define OPC_nGFAD		0102	// Giant Floating Add
#define OPC_nGFSB		0103	// Giant Floating Subtract

#define OPC_nJSYS		0104
#define OPC_nADJSP		0105
#define OPC_nGMPV		0106	// Giant Floating Multiply
#define OPC_nGFDV		0107	// Giant Floating Divide

#define OPC_nDFAD		0110	// Double Floating Add
#define OPC_nDFSB		0111	// Double Floating Subtract
#define OPC_nDFMP		0112	// Double Floating Multiply
#define OPC_nDFDV		0113	// Double Floating Divide

#define OPC_nDADD		0114	// Double Add
#define OPC_nDSUB		0115	// Double Subtract
#define OPC_nDMUL		0116	// Double Multiply
#define OPC_nDDIV		0117	// Double Divide

#define OPC_nDMOVE		0120	// Double Move
#define OPC_nDMOVN		0121	// Double Move Negative
#define OPC_nFIX		0122	// Fix
#define OPC_nEXTEND		0123	// Extended Instruction

#define OPC_nDMOVEM		0124	// Double Move to Memory
#define OPC_nDMOVNM		0125	// Double Move Negative to Memory
#define OPC_nFIXR		0126	// Fix and Round
#define OPC_nFLTR		0127	// Float and Round

#define OPC_nUFA		0130	// Unnormalized Floating Add
#define OPC_nDFN		0131	// Double Floating Negate
#define OPC_nFSC		0132	// Floating Scale
#define OPC_nADJBP		0133
#define OPC_nIBP		0133

#define OPC_nILDB		0134
#define OPC_nLDB		0135
#define OPC_nIDPB		0136
#define OPC_nDPB		0137

#define OPC_nFAD		0140	// Floating Add
#define OPC_nFADL		0141	// Floating Add Long
#define OPC_nFADM		0142	// Floating Add to Memory
#define OPC_nFADB		0143	// Floating Add to Both

#define OPC_nFADR		0144	// Floating Add and Round
#define OPC_nFADRI		0145	// Floating Add and Round Immediate
#define OPC_nFADRM		0146	// Floating Add and Round to Memory
#define OPC_nFADRB		0147	// Floating Add and Round to Both

#define OPC_nFSB		0150	// Floating Subtract
#define OPC_nFSBL		0151	// Floating Subtract Long
#define OPC_nFSBM		0152	// Floating Subtract to Memory
#define OPC_nFSBB		0153	// Floating Subtract to Both

#define OPC_nFSBR		0154	// Floating Subtract and Round
#define OPC_nFSBRI		0155	// Floating Subtract and Round Immediate
#define OPC_nFSBRM		0156	// Floating Subtract and Round to Memory
#define OPC_nFSBRB		0157	// Floating Subtract and Round to Both

#define OPC_nFMP		0160	// Floating Multiply
#define OPC_nFMPL		0161	// Floating Multiply Long
#define OPC_nFMPM		0162	// Floating Multiply to Memory
#define OPC_nFMPB		0163	// Floating Multiply to Both

#define OPC_nFMPR		0164	// Floating Multiply and Round
#define OPC_nFMPRI		0165	// Floating Multiply and Round Immediate
#define OPC_nFMPRM		0166	// Floating Multiply and Round to Memory
#define OPC_nFMPRB		0167	// Floating Multiply and Round to Both

#define OPC_nFDV		0170	// Floating Divide
#define OPC_nFDVL		0171	// Floating Divide Long
#define OPC_nFDVM		0172	// Floating Divide to Memory
#define OPC_nFDVB		0173	// Floating Divide to Both

#define OPC_nFDVR		0174	// Floating Divide and Round
#define OPC_nFDVRI		0175	// Floating Divide and Round Immediate
#define OPC_nFDVRM		0176	// Floating Divide and Round to Memory
#define OPC_nFDVRB		0177	// Floating Divide and Round to Both


#define OPC_nMOVE		0200	// Move
#define OPC_nMOVEI		0201	// Move Immediate
#define OPC_nMOVEM		0202	// Move to Memory
#define OPC_nMOVES		0203	// Move to Self

#define OPC_nMOVS		0204	// Move Swapped
#define OPC_nMOVSI		0205	// Move Swapped Immediate
#define OPC_nMOVSM		0206	// Move Swapped to Memory
#define OPC_nMOVSS		0207	// Move Swapped to Self

#define OPC_nMOVN		0210	// Move Negative
#define OPC_nMOVNI		0211	// Move Negative Immediate
#define OPC_nMOVNM		0212	// Move Negative to Memory
#define OPC_nMOVNS		0213	// Move Negative to Self

#define OPC_nMOVM		0214	// Move Magnitude
#define OPC_nMOVMI		0215	// Move Magnitude Immediate
#define OPC_nMOVMM		0216	// Move Magnitude to Memory
#define OPC_nMOVMS		0217	// Move Magnitude to Self

#define OPC_nIMUL		0220	// Integer Multiply
#define OPC_nIMULI		0221	// Integer Multiply Immediate
#define OPC_nIMULM		0222	// Integer Multiply to Memory
#define OPC_nIMULB		0223	// Integer Multiply to Both

#define OPC_nMUL		0224	// Multiply
#define OPC_nMULI		0225	// Multiply Immediate
#define OPC_nMULM		0226	// Multiply to Memory
#define OPC_nMULB		0227	// Multiply to Both

#define OPC_nIDIV		0230	// Integer Divide
#define OPC_nIDIVI		0231	// Integer Divide Immediate
#define OPC_nIDIVM		0232	// Integer Divide to Memory
#define OPC_nIDIVB		0233	// Integer Divide to Both

#define OPC_nDIV		0234	// Divide
#define OPC_nDIVI		0235	// Divide Immediate
#define OPC_nDIVM		0236	// Divide to Memory
#define OPC_nDIVB		0237	// Divide to Both

#define OPC_nASH		0240
#define OPC_nROT		0241
#define OPC_nLSH		0242
#define OPC_nJFFO		0243

#define OPC_nASHC		0244
#define OPC_nROTC		0245
#define OPC_nLSHC		0246

#define OPC_nEXCH		0250	// Exchange
#define OPC_nBLT		0251	// Block Transfer
#define OPC_nAOBJP		0252
#define OPC_nAOBJN		0253

#define OPC_nJRST		0254
#define OPC_nJFCL		0255
#define OPC_nXCT		0256
#define OPC_nMAP		0257

#define OPC_nPUSHJ		0260
#define OPC_nPUSH		0261
#define OPC_nPOP		0262
#define OPC_nPOPJ		0263

#define OPC_nJSR		0264
#define OPC_nJSP		0265
#define OPC_nJSA		0266
#define OPC_nJRA		0267

#define OPC_nADD		0270	// Add
#define OPC_nADDI		0271	// Add Immediate
#define OPC_nADDM		0272	// Add to Memory
#define OPC_nADDB		0273	// Add to Both

#define OPC_nSUB		0274	// Subtract
#define OPC_nSUBI		0275	// Subtract Immediate
#define OPC_nSUBM		0276	// Subtract to Memory
#define OPC_nSUBB		0277	// Subtract to Both


#define OPC_nCAI		0300
#define OPC_nCAIL		0301
#define OPC_nCAIE		0302
#define OPC_nCAILE		0303
#define OPC_nCAIA		0304
#define OPC_nCAIGE		0305
#define OPC_nCAIN		0306
#define OPC_nCAIG		0307

#define OPC_nCAM		0310
#define OPC_nCAML		0311
#define OPC_nCAME		0312
#define OPC_nCAMLE		0313
#define OPC_nCAMA		0314
#define OPC_nCAMGE		0315
#define OPC_nCAMN		0316
#define OPC_nCAMG		0317

#define OPC_nJUMP		0320
#define OPC_nJUMPL		0321
#define OPC_nJUMPE		0322
#define OPC_nJUMPLE		0323
#define OPC_nJUMPA		0324
#define OPC_nJUMPGE		0325
#define OPC_nJUMPN		0326
#define OPC_nJUMPG		0327

#define OPC_nSKIP		0330
#define OPC_nSKIPL		0331
#define OPC_nSKIPE		0332
#define OPC_nSKIPLE		0333
#define OPC_nSKIPA		0334
#define OPC_nSKIPGE		0335
#define OPC_nSKIPN		0336
#define OPC_nSKIPG		0337

#define OPC_nAOJ		0340
#define OPC_nAOJL		0341
#define OPC_nAOJE		0342
#define OPC_nAOJLE		0343
#define OPC_nAOJA		0344
#define OPC_nAOJGE		0345
#define OPC_nAOJN		0346
#define OPC_nAOJG		0347

#define OPC_nAOS		0350
#define OPC_nAOSL		0351
#define OPC_nAOSE		0352
#define OPC_nAOSLE		0353
#define OPC_nAOSA		0354
#define OPC_nAOSGE		0355
#define OPC_nAOSN		0356
#define OPC_nAOSG		0357

#define OPC_nSOJ		0360
#define OPC_nSOJL		0361
#define OPC_nSOJE		0362
#define OPC_nSOJLE		0363
#define OPC_nSOJA		0364
#define OPC_nSOJGE		0365
#define OPC_nSOJN		0366
#define OPC_nSOJG		0367

#define OPC_nSOS		0370
#define OPC_nSOSL		0371
#define OPC_nSOSE		0372
#define OPC_nSOSLE		0373
#define OPC_nSOSA		0374
#define OPC_nSOSGE		0375
#define OPC_nSOSN		0376
#define OPC_nSOSG		0377


#define OPC_nSETZ		0400
#define OPC_nSETZI		0401
#define OPC_nSETZM		0402
#define OPC_nSETZB		0403

#define OPC_nAND		0404
#define OPC_nANDI		0405
#define OPC_nANDM		0406
#define OPC_nANDB		0407

#define OPC_nANDCA		0410
#define OPC_nANDCAI		0411
#define OPC_nANDCAM		0412
#define OPC_nANDCAB		0413

#define OPC_nSETM		0414
#define OPC_nSETMI		0415
#define OPC_nXMOVEI		0415	// Extended Move Immediate
#define OPC_nSETMM		0416
#define OPC_nSETMB		0417

#define OPC_nANDCM		0420
#define OPC_nANDCMI		0421
#define OPC_nANDCMM		0422
#define OPC_nANDCMB		0423

#define OPC_nSETA		0424
#define OPC_nSETAI		0425
#define OPC_nSETAM		0426
#define OPC_nSETAB		0427

#define OPC_nXOR		0430
#define OPC_nXORI		0431
#define OPC_nXORM		0432
#define OPC_nXORB		0433

#define OPC_nIOR		0434
#define OPC_nIORI		0435
#define OPC_nIORM		0436
#define OPC_nIORB		0437

#define OPC_nANDCB		0440
#define OPC_nANDCBI		0441
#define OPC_nANDCBM		0442
#define OPC_nANDCBB		0443

#define OPC_nEQV		0444
#define OPC_nEQVI		0445
#define OPC_nEQVM		0446
#define OPC_nEQVB		0447

#define OPC_nSETCA		0450
#define OPC_nSETCAI		0451
#define OPC_nSETCAM		0452
#define OPC_nSETCAB		0453

#define OPC_nORA		0454
#define OPC_nORAI		0455
#define OPC_nORAM		0456
#define OPC_nORAB		0457

#define OPC_nSETCM		0460
#define OPC_nSETCMI		0461
#define OPC_nSETCMM		0462
#define OPC_nSETCMB		0463

#define OPC_nORCM		0464
#define OPC_nORCMI		0465
#define OPC_nORCMM		0466
#define OPC_nORCMB		0467

#define OPC_nORCB		0470
#define OPC_nORCBI		0471
#define OPC_nORCBM		0472
#define OPC_nORCBB		0473

#define OPC_nSETO		0474
#define OPC_nSETOI		0475
#define OPC_nSETOM		0476
#define OPC_nSETOB		0477


#define OPC_nHLL		0500
#define OPC_nHLLI		0501
#define OPC_nXHLLI		0501
#define OPC_nHLLM		0502
#define OPC_nHLLS		0503

#define OPC_nHRL		0504
#define OPC_nHRLI		0505
#define OPC_nHRLM		0506
#define OPC_nHRLS		0507

#define OPC_nHLLZ		0510
#define OPC_nHLLZI		0511
#define OPC_nHLLZM		0512
#define OPC_nHLLZS		0513

#define OPC_nHRLZ		0514
#define OPC_nHRLZI		0515
#define OPC_nHRLZM		0516
#define OPC_nHRLZS		0517

#define OPC_nHLLO		0520
#define OPC_nHLLOI		0521
#define OPC_nHLLOM		0522
#define OPC_nHLLOS		0523

#define OPC_nHRLO		0524
#define OPC_nHRLOI		0525
#define OPC_nHRLOM		0526
#define OPC_nHRLOS		0527

#define OPC_nHLLE		0530
#define OPC_nHLLEI		0531
#define OPC_nHLLEM		0532
#define OPC_nHLLES		0533

#define OPC_nHRLE		0534
#define OPC_nHRLEI		0535
#define OPC_nHRLEM		0536
#define OPC_nHRLES		0537

#define OPC_nHRR		0540
#define OPC_nHRRI		0541
#define OPC_nHRRM		0542
#define OPC_nHRRS		0543

#define OPC_nHLR		0544
#define OPC_nHLRI		0545
#define OPC_nHLRM		0546
#define OPC_nHLRS		0547

#define OPC_nHRRZ		0550
#define OPC_nHRRZI		0551
#define OPC_nHRRZM		0552
#define OPC_nHRRZS		0553

#define OPC_nHLRZ		0554
#define OPC_nHLRZI		0555
#define OPC_nHLRZM		0556
#define OPC_nHLRZS		0557

#define OPC_nHRRO		0560
#define OPC_nHRROI		0561
#define OPC_nHRROM		0562
#define OPC_nHRROS		0563

#define OPC_nHLRO		0564
#define OPC_nHLROI		0565
#define OPC_nHLROM		0566
#define OPC_nHLROS		0567

#define OPC_nHRRE		0570
#define OPC_nHRREI		0571
#define OPC_nHRREM		0572
#define OPC_nHRRES		0573

#define OPC_nHLRE		0574
#define OPC_nHLREI		0575
#define OPC_nHLREM		0576
#define OPC_nHLRES		0577


#define OPC_nTRN		0600
#define OPC_nTLN		0601
#define OPC_nTRNE		0602
#define OPC_nTLNE		0603
#define OPC_nTRNA		0604
#define OPC_nTLNA		0605
#define OPC_nTRNN		0606
#define OPC_nTLNN		0607

#define OPC_nTDN		0610
#define OPC_nTSN		0611
#define OPC_nTDNE		0612
#define OPC_nTSNE		0613
#define OPC_nTDNA		0614
#define OPC_nTSNA		0615
#define OPC_nTDNN		0616
#define OPC_nTSNN		0617

#define OPC_nTRZ		0620
#define OPC_nTLZ		0621
#define OPC_nTRZE		0622
#define OPC_nTLZE		0623
#define OPC_nTRZA		0624
#define OPC_nTLZA		0625
#define OPC_nTRZN		0626
#define OPC_nTLZN		0627

#define OPC_nTDZ		0630
#define OPC_nTSZ		0631
#define OPC_nTDZE		0632
#define OPC_nTSZE		0633
#define OPC_nTDZA		0634
#define OPC_nTSZA		0635
#define OPC_nTDZN		0636
#define OPC_nTSZN		0637

#define OPC_nTRC		0640
#define OPC_nTLC		0641
#define OPC_nTRCE		0642
#define OPC_nTLCE		0643
#define OPC_nTRCA		0644
#define OPC_nTLCA		0645
#define OPC_nTRCN		0646
#define OPC_nTLCN		0647

#define OPC_nTDC		0650
#define OPC_nTSC		0651
#define OPC_nTDCE		0652
#define OPC_nTSCE		0653
#define OPC_nTDCA		0654
#define OPC_nTSCA		0655
#define OPC_nTDCN		0656
#define OPC_nTSCN		0657

#define OPC_nTRO		0660
#define OPC_nTLO		0661
#define OPC_nTROE		0662
#define OPC_nTLOE		0663
#define OPC_nTROA		0664
#define OPC_nTLOA		0665
#define OPC_nTRON		0666
#define OPC_nTLON		0667

#define OPC_nTDO		0670
#define OPC_nTSO		0671
#define OPC_nTDOE		0672
#define OPC_nTSOE		0673
#define OPC_nTDOA		0674
#define OPC_nTSOA		0675
#define OPC_nTDON		0676
#define OPC_nTSON		0677

#define OPC_nAPR0		0700
#define OPC_nAPR1		0701
#define OPC_nAPR2		0702
#define OPC_nAPR3		0703

#define OPC_nPMOVE		0704
#define OPC_nPMOVEM		0705
#define OPC_nNMOVE		0706
#define OPC_nNMOVEM		0707

#define OPC_nLDCFG		0710
#define OPC_nRDCFG		0711

#define OPC_nAMOVE		0714
#define OPC_nAMOVEM		0715
#define OPC_nUMOVE		0716
#define OPC_nUMOVEM		0717

// EXTEND instructions
#define OPC_nCMPSL		0001
#define OPC_nCMPSE		0002
#define OPC_nCMPSLE		0003

#define OPC_nEDIT		0004
#define OPC_nCMPSGE		0005
#define OPC_nCMPSN		0006
#define OPC_nCMPSG		0007

#define OPC_nCVTDBO		0010
#define OPC_nCVTDBT		0011
#define OPC_nCVTBDO		0012
#define OPC_nCVTBDT		0013

#define OPC_nMOVSO		0014
#define OPC_nMOVST		0015
#define OPC_nMOVSLJ		0016
#define OPC_nMOVSRJ		0017

#define OPC_nXBLT		0020
#define OPC_nGSNGL		0021
#define OPC_nGDBLE		0022
#define OPC_nDGFIX		0023

#define OPC_nGFIX		0024
#define OPC_nDGFIXR		0025
#define OPC_nGFIXR		0026
#define OPC_nDGFLTR		0027

#define OPC_nGFLTR		0030
#define OPC_nGFSC		0031

// Operand flags
#define OPR_SWAR		000001 // Swap AR operand
#define OPR_FEMB		000002 // Fetch from Memory
#define OPR_SVAR		000004 // Save AR operand to Memory
