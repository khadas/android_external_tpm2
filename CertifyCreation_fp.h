/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CERTIFYCREATION_FP_H
#define __TPM2_CERTIFYCREATION_FP_H

typedef struct {
        TPM2B_DIGEST creationHash;
        TPMT_TK_CREATION creationTicket;
        TPMT_SIG_SCHEME inScheme;
        TPMI_DH_OBJECT objectHandle;
        TPM2B_DATA qualifyingData;
        TPMI_DH_OBJECT signHandle;
} CertifyCreation_In;

typedef struct {
        TPM2B_ATTEST    certifyInfo;
        TPMT_SIGNATURE  signature;
} CertifyCreation_Out;

TPM_RC TPM2_CertifyCreation(
        CertifyCreation_In     *in,                // IN: input parameter list
        CertifyCreation_Out    *out                // OUT: output parameter list
);

#endif // __TPM2_CERTIFYCREATION_FP_H
