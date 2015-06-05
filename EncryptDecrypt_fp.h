/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_ENCRYPTDECRYPT_FP_H
#define __TPM2_ENCRYPTDECRYPT_FP_H

typedef struct {
  TPMI_DH_OBJECT keyHandle;
  TPM_ALG_ID mode;
  TPM2B_IV ivIn;
  TPM2B_DATA inData;
  TPMI_YES_NO decrypt;
} EncryptDecrypt_In;

typedef struct {
  TPM2B_IV ivOut;
  TPM2B_DATA outData;
} EncryptDecrypt_Out;

TPM_RC TPM2_EncryptDecrypt(
    EncryptDecrypt_In *in,   // IN: input parameter list
    EncryptDecrypt_Out *out  // OUT: output parameter list
    );

#endif  // __TPM2_ENCRYPTDECRYPT_FP_H
