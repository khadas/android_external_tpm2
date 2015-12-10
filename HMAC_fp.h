/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_HMAC_FP_H
#define __TPM2_HMAC_FP_H

typedef struct {
        TPMI_DH_OBJECT  handle;
        TPM_ALG_ID      hashAlg;
        TPM2B_DATA      buffer;
} HMAC_In;

typedef struct {
        TPM2B_AUTH      outHMAC;
} HMAC_Out;

TPM_RC TPM2_HMAC(
        HMAC_In           *in,                 // IN: input parameter list
        HMAC_Out          *out                 // OUT: output parameter list
                 );

#endif // __TPM2_HMAC_FP_H
