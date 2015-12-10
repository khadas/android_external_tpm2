/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ZGEN_2PHASE_FP_H_
#define __TPM2_ZGEN_2PHASE_FP_H_

typedef struct {
  TPMI_DH_OBJECT keyA;
  UINT16 counter;
  TPM_ALG_ID inScheme;
  TPM2B_ECC_POINT inQeB;
  TPM2B_ECC_POINT inQsB;
} ZGen_2Phase_In;

typedef struct {
  TPM2B_ECC_POINT outZ1;
  TPM2B_ECC_POINT outZ2;
} ZGen_2Phase_Out;

TPM_RC TPM2_ZGen_2Phase(ZGen_2Phase_In *in,   // IN: input parameter list
                        ZGen_2Phase_Out *out  // OUT: output parameter list
                        );

#endif  // __TPM2_ZGEN_2PHASE_FP_H_
