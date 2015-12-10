/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_QUOTE_FP_H_
#define __TPM2_QUOTE_FP_H_

typedef struct {
        TPMI_DH_OBJECT      signHandle;
        TPMT_SIG_SCHEME     inScheme;
        TPM2B_DATA          qualifyingData;
        TPML_PCR_SELECTION  PCRselect;
} Quote_In;

typedef struct {
        TPM2B_ATTEST   quoted;
        TPMT_SIGNATURE signature;
} Quote_Out;

TPM_RC TPM2_Quote(
        Quote_In        *in,             // IN: input parameter list
        Quote_Out       *out             // OUT: output parameter list
);

#endif // __TPM2_QUOTE_FP_H_
