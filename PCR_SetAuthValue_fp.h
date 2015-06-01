/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_PCR_SETAUTHVALUE_FP_H_
#define __TPM2_PCR_SETAUTHVALUE_FP_H_

typedef struct {
        TPMI_DH_PCR  pcrHandle;
        TPM2B_DIGEST auth;
} PCR_SetAuthValue_In;

TPM_RC TPM2_PCR_SetAuthValue(
        PCR_SetAuthValue_In       *in              // IN: input parameter list
);

#endif // __TPM2_PCR_SETAUTHVALUE_FP_H_
