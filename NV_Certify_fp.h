/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_CERTIFY_FP_H
#define __TPM2_NV_CERTIFY_FP_H
typedef struct {
  TPMI_RH_NV_INDEX nvIndex;
  TPM_HANDLE authHandle;
  UINT32 offset;
  UINT16 size;
  TPMI_DH_OBJECT signHandle;
  TPMT_SIG_SCHEME inScheme;
  TPM2B_DATA qualifyingData;
} NV_Certify_In;

typedef struct {
  TPM2B_ATTEST certifyInfo;
  TPMT_SIGNATURE signature;
} NV_Certify_Out;

TPM_RC TPM2_NV_Certify(NV_Certify_In *in,   // IN: input parameter list
                       NV_Certify_Out *out  // OUT: output parameter list
                       );

#endif  // __TPM2_NV_CERTIFY_FP_H
