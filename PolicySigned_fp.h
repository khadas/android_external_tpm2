/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_POLICYSIGNED_FP_H_
#define __TPM2_POLICYSIGNED_FP_H_

typedef struct {
  TPM_HANDLE policySession;
  TPMI_DH_OBJECT authObject;
  INT32 expiration;
  TPM2B_NONCE policyRef;
  TPM2B_DIGEST cpHashA;
  TPM2B_NONCE nonceTPM;
  TPMT_SIGNATURE auth;
} PolicySigned_In;

typedef struct {
  TPM2B_DATA timeout;
  TPMT_TK_AUTH policyTicket;
} PolicySigned_Out;

TPM_RC TPM2_PolicySigned(PolicySigned_In *in,   // IN: input parameter list
                         PolicySigned_Out *out  // OUT: output parameter list
                         );

#endif  // __TPM2_POLICYSIGNED_FP_H_
