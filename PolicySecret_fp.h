/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_POLICYSECRET_FP_H_
#define __TPM2_POLICYSECRET_FP_H_

typedef struct {
        TPM_HANDLE      policySession;
        INT32           expiration;
        TPM2B_NONCE     policyRef;
        TPM2B_DIGEST    cpHashA;
        TPM2B_NONCE     nonceTPM;
        TPMI_DH_ENTITY  authHandle;
} PolicySecret_In;

typedef struct {
        TPMT_TK_AUTH policyTicket;
        TPM2B_DATA   timeout;
} PolicySecret_Out;

TPM_RC TPM2_PolicySecret(
        PolicySecret_In    *in,                 // IN: input parameter list
        PolicySecret_Out   *out                 // OUT: output parameter list
);

#endif // __TPM2_POLICYSECRET_FP_H_
