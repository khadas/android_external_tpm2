/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_GETTIME_FP_H
#define __TPM2_GETTIME_FP_H

typedef struct {
        TPMI_DH_OBJECT  signHandle;     //   IN: handle of signing object
        TPMT_SIG_SCHEME inScheme;       //   IN/OUT: scheme to be used for signing
        TPM2B_DATA      qualifyingData; //   IN: qualifying data
} GetTime_In;

typedef struct {
        TPM2B_ATTEST    timeInfo;
        TPMT_SIGNATURE  signature;
} GetTime_Out;

TPM_RC TPM2_GetTime(
        GetTime_In      *in,             // IN: input parameter list
        GetTime_Out     *out             // OUT: output parameter list
                    );

#endif // __TPM2_GETTIME_FP_H
