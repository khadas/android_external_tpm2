/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_MAKECREDENTIAL_FP_H
#define __TPM2_MAKECREDENTIAL_FP_H

typedef struct {
  TPMI_DH_OBJECT handle;
  TPM2B_DIGEST credential;
  TPM2B_NAME objectName;
} MakeCredential_In;

typedef struct {
  TPM2B_ID_OBJECT credentialBlob;
  TPM2B_ENCRYPTED_SECRET secret;  //   OUT: secret structure
} MakeCredential_Out;

TPM_RC TPM2_MakeCredential(
    MakeCredential_In *in,   // IN: input parameter list
    MakeCredential_Out *out  // OUT: output parameter list
    );

#endif  // __TPM2_MAKECREDENTIAL_FP_H
