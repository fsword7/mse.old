/*
 * opcode.h
 *
 *  Created on: Feb 12, 2017
 *      Author: fswor
 */

#pragma once

#define OPC_nUUO		0000
#define OPC_nLUUO01		0001
#define OPC_nLUUO02		0001


#define OPC_nDFAD		0110
#define OPC_nDFSB		0111
#define OPC_nDFMP		0112
#define OPC_nDFDV		0113

#define OPC_nDADD		0114
#define OPC_nDSUB		0115
#define OPC_nDMUL		0116
#define OPC_nDDIV		0117

#define OPC_nDMOVE		0120
#define OPC_nDMOVN		0121
#define OPC_nFIX		0122

#define OPC_nDMOVEM		0124
#define OPC_nDMOVNM		0125
#define OPC_nFIXR		0126
#define OPC_nFLTR		0127

#define OPC_nUFA		0130
#define OPC_nDFN		0131
#define OPC_nFSC		0132
#define OPC_nADJBP		0133
#define OPC_nIBP		0133

#define OPC_nILDB		0134
#define OPC_nLDB		0135
#define OPC_nIDPB		0136
#define OPC_nDPB		0137

#define OPC_nFAD		0140
#define OPC_nFADL		0141
#define OPC_nFADM		0142
#define OPC_nFADB		0143

#define OPC_nFADR		0144
#define OPC_nFADRI		0145
#define OPC_nFADRM		0146
#define OPC_nFADRB		0147

#define OPC_nFSB		0150
#define OPC_nFSBL		0151
#define OPC_nFSBM		0152
#define OPC_nFSBB		0153

#define OPC_nFSBR		0154
#define OPC_nFSBRI		0155
#define OPC_nFSBRM		0156
#define OPC_nFSBRB		0157

#define OPC_nFMP		0160
#define OPC_nFMPL		0161
#define OPC_nFMPM		0162
#define OPC_nFMPB		0163

#define OPC_nFMPR		0164
#define OPC_nFMPRI		0165
#define OPC_nFMPRM		0166
#define OPC_nFMPRB		0167

#define OPC_nFDV		0170
#define OPC_nFDVL		0171
#define OPC_nFDVM		0172
#define OPC_nFDVB		0173

#define OPC_nFDVR		0174
#define OPC_nFDVRI		0175
#define OPC_nFDVRM		0176
#define OPC_nFDVRB		0177


#define OPC_nMOVE		0200
#define OPC_nMOVEI		0201
#define OPC_nMOVEM		0202
#define OPC_nMOVES		0203

#define OPC_nMOVS		0204
#define OPC_nMOVSI		0205
#define OPC_nMOVSM		0206
#define OPC_nMOVSS		0207

#define OPC_nMOVN		0210
#define OPC_nMOVNI		0211
#define OPC_nMOVNM		0212
#define OPC_nMOVNS		0213

#define OPC_nMOVM		0214
#define OPC_nMOVMI		0215
#define OPC_nMOVMM		0216
#define OPC_nMOVMS		0217

#define OPC_nIMUL		0220
#define OPC_nIMULI		0221
#define OPC_nIMULM		0222
#define OPC_nIMULB		0223

#define OPC_nMUL		0224
#define OPC_nMULI		0225
#define OPC_nMULM		0226
#define OPC_nMULB		0227

#define OPC_nIDIV		0230
#define OPC_nIDIVI		0231
#define OPC_nIDIVM		0232
#define OPC_nIDIVB		0233

#define OPC_nDIV		0234
#define OPC_nDIVI		0235
#define OPC_nDIVM		0236
#define OPC_nDIVB		0237

#define OPC_nASH		0240
#define OPC_nROT		0241
#define OPC_nLSH		0242
#define OPC_nJFFO		0243

#define OPC_nASHC		0244
#define OPC_nROTC		0245
#define OPC_nLSHC		0246

#define OPC_nEXCH		0250
#define OPC_nBLT		0251
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

#define OPC_nADD		0270
#define OPC_nADDI		0271
#define OPC_nADDM		0272
#define OPC_nADDB		0273

#define OPC_nSUB		0274
#define OPC_nSUBI		0275
#define OPC_nSUBM		0276
#define OPC_nSUBB		0277


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

