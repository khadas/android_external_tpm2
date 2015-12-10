/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_STARTAUTHSESSION_FP_H_
#define __TPM2_STARTAUTHSESSION_FP_H_

typedef struct {
        TPM_HANDLE              tpmKey;
        TPM_SE                  sessionType;
        TPMI_ALG_HASH           authHash;
        TPM2B_NONCE             nonceCaller;
        TPMT_SYM_DEF            symmetric;
        TPMI_DH_ENTITY          bind;
        TPM2B_ENCRYPTED_SECRET  encryptedSalt;
} StartAuthSession_In;

typedef struct {
        TPM2B_NONCE  nonceTPM;
        TPM_HANDLE   sessionHandle;
} StartAuthSession_Out;

TPM_RC TPM2_StartAuthSession(
        StartAuthSession_In       *in,                   // IN: input parameter buffer
        StartAuthSession_Out      *out                   // OUT: output parameter buffer
);

#endif // __TPM2_STARTAUTHSESSION_FP_H_
