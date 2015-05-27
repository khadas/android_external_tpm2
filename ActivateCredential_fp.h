/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ACTIVATECREDENTIAL_FP_H
#define __TPM2_ACTIVATECREDENTIAL_FP_H

typedef struct {
        TPMI_DH_OBJECT keyHandle;
        TPMI_DH_OBJECT activateHandle;
        TPM2B_ENCRYPTED_SECRET secret;
        TPM2B_ID_OBJECT credentialBlob;
} ActivateCredential_In;

typedef struct {
        TPM2B_DIGEST certInfo;
} ActivateCredential_Out;

TPM_RC TPM2_ActivateCredential(
        ActivateCredential_In    *in,                 // IN: input parameter list
        ActivateCredential_Out   *out                 // OUT: output parameter list
);

#endif // __TPM2_ACTIVATECREDENTIAL_FP_H
