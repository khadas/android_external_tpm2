/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_RSA_ENCRYPT_FP_H_
#define __TPM2_RSA_ENCRYPT_FP_H_

typedef struct {
        TPMI_DH_OBJECT    keyHandle;
        TPM2B_DATA        label;
        TPM2B_DATA        message;
        TPMT_RSA_DECRYPT  inScheme;
} RSA_Encrypt_In;

typedef struct {
        TPM2B_DATA  outData;
} RSA_Encrypt_Out;

TPM_RC TPM2_RSA_Encrypt(
        RSA_Encrypt_In        *in,                 // IN: input parameter list
        RSA_Encrypt_Out       *out                 // OUT: output parameter list
);

#endif // __TPM2_RSA_ENCRYPT_FP_H_
