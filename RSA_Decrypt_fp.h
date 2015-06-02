/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_RSA_DECRYPT_FP_H_
#define __TPM2_RSA_DECRYPT_FP_H_

typedef struct {
        TPM_HANDLE        keyHandle;
        TPM2B_DATA        cipherText;
        TPM2B_DATA        label;
        TPMT_RSA_DECRYPT  inScheme;
} RSA_Decrypt_In;

typedef struct {
        TPM2B_DATA        message;
} RSA_Decrypt_Out;

TPM_RC TPM2_RSA_Decrypt(
        RSA_Decrypt_In        *in,                   // IN: input parameter list
        RSA_Decrypt_Out       *out                   // OUT: output parameter list
);

#endif // __TPM2_RSA_DECRYPT_FP_H_
