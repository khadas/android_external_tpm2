/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_GETCOMMANDAUDITDIGEST_FP_H
#define __TPM2_GETCOMMANDAUDITDIGEST_FP_H

typedef struct {
        TPMI_DH_OBJECT  signHandle;
        TPMT_SIG_SCHEME inScheme;
        TPM2B_DATA      qualifyingData;
} GetCommandAuditDigest_In;

typedef struct {
        TPM2B_ATTEST    auditInfo;
        TPMT_SIGNATURE  signature;
} GetCommandAuditDigest_Out;

TPM_RC TPM2_GetCommandAuditDigest(
        GetCommandAuditDigest_In      *in,                // IN: input parameter list
        GetCommandAuditDigest_Out     *out                // OUT: output parameter list
);

#endif // __TPM2_GETCOMMANDAUDITDIGEST_FP_H
