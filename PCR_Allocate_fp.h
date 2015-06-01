/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_PCR_ALLOCATE_FP_H_
#define __TPM2_PCR_ALLOCATE_FP_H_

typedef struct {
        TPML_PCR_SELECTION pcrAllocation;
} PCR_Allocate_In;

typedef struct {
        UINT32      maxPCR;
        UINT32      sizeNeeded;
        UINT32      sizeAvailable;
        TPMI_YES_NO allocationSuccess;
} PCR_Allocate_Out;

TPM_RC TPM2_PCR_Allocate(
        PCR_Allocate_In       *in,                 // IN: input parameter list
        PCR_Allocate_Out      *out                 // OUT: output parameter list
);

#endif // __TPM2_PCR_ALLOCATE_FP_H_
