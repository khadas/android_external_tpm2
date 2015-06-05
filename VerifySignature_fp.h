/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_VERIFYSIGNATURE_FP_H_
#define __TPM2_VERIFYSIGNATURE_FP_H_

typedef struct {
  TPMI_DH_OBJECT keyHandle;
  TPM2B_DIGEST digest;
  TPMT_SIGNATURE signature;
} VerifySignature_In;

typedef struct { TPMT_TK_VERIFIED validation; } VerifySignature_Out;

TPM_RC TPM2_VerifySignature(
    VerifySignature_In *in,   // IN: input parameter list
    VerifySignature_Out *out  // OUT: output parameter list
    );

#endif  // __TPM2_VERIFYSIGNATURE_FP_H_
