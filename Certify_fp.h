/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CERTIFY_FP_H
#define __TPM2_CERTIFY_FP_H

typedef struct {
        TPMI_DH_OBJECT  signHandle;
        TPMI_DH_OBJECT  objectHandle;
        TPMT_SIG_SCHEME inScheme;
        TPM2B_DATA      qualifyingData;
} Certify_In;

typedef struct {
        TPM2B_ATTEST    certifyInfo;
        TPMT_SIGNATURE  signature;
} Certify_Out;

TPM_RC TPM2_Certify(
        Certify_In      *in,  // IN: input parameter list
        Certify_Out     *out  // OUT: output parameter list
);

#endif // __TPM2_CERTIFY_FP_H
