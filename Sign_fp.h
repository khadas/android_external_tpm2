/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_SIGN_FP_H_
#define __TPM2_SIGN_FP_H_

typedef struct {
        TPMI_DH_OBJECT  keyHandle;
        TPMT_SIG_SCHEME inScheme;
        TPM2B_DIGEST    digest;
        TPMT_TK_COMMON  validation;
} Sign_In;

typedef struct {
        TPMT_SIGNATURE  signature;
} Sign_Out;

TPM_RC TPM2_Sign(
        Sign_In          *in,                   // IN: input parameter list
        Sign_Out         *out                   // OUT: output parameter list
);

#endif // __TPM2_SIGN_FP_H_
