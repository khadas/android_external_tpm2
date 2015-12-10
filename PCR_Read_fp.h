/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_PCR_READ_FP_H_
#define __TPM2_PCR_READ_FP_H_

typedef struct { TPML_PCR_SELECTION pcrSelectionIn; } PCR_Read_In;

typedef struct {
  TPML_DIGEST pcrValues;
  UINT32 pcrUpdateCounter;
  TPML_PCR_SELECTION pcrSelectionOut;
} PCR_Read_Out;

TPM_RC TPM2_PCR_Read(PCR_Read_In *in,   // IN: input parameter list
                     PCR_Read_Out *out  // OUT: output parameter list
                     );

#endif  // __TPM2_PCR_READ_FP_H_
