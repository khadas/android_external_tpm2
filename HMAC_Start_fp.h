/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_HMAC_START_FP_H
#define __TPM2_HMAC_START_FP_H

typedef struct {
        TPM_HANDLE      handle;
        TPM_ALG_ID      hashAlg;
        TPM2B_AUTH      auth;
} HMAC_Start_In;

typedef struct {
        TPMI_DH_OBJECT     sequenceHandle;
} HMAC_Start_Out;

TPM_RC TPM2_HMAC_Start(
        HMAC_Start_In     *in,                 // IN: input parameter list
        HMAC_Start_Out    *out                 // OUT: output parameter list
                       );

#endif // __TPM2_HMAC_START_FP_H
