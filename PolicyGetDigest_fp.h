/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_POLICYGETDIGEST_FP_H_
#define __TPM2_POLICYGETDIGEST_FP_H_

typedef struct {
        TPM_HANDLE   policySession;
} PolicyGetDigest_In;

typedef struct {
        TPM2B_DIGEST policyDigest;
} PolicyGetDigest_Out;

TPM_RC TPM2_PolicyGetDigest(
        PolicyGetDigest_In        *in,             // IN: input parameter list
        PolicyGetDigest_Out       *out             // OUT: output parameter list
);

#endif // __TPM2_POLICYGETDIGEST_FP_H_
