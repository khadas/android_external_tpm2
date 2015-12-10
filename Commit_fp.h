/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_COMMIT_FP_H
#define __TPM2_COMMIT_FP_H

typedef struct {
  TPM2B_ECC_PARAMETER s2;
  TPM2B_ECC_PARAMETER y2;
  TPM2B_ECC_POINT P1;
  TPMI_DH_OBJECT signHandle;
} Commit_In;

typedef struct {
  TPM2B_ECC_POINT E;
  TPM2B_ECC_POINT K;
  TPM2B_ECC_POINT L;
  UINT16 counter;
} Commit_Out;

TPM_RC TPM2_Commit(Commit_In *in,   // IN: input parameter list
                   Commit_Out *out  // OUT: output parameter list
                   );

#endif  // __TPM2_COMMIT_FP_H
