/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_SEQUENCECOMPLETE_FP_H_
#define __TPM2_SEQUENCECOMPLETE_FP_H_

typedef struct {
        TPM_RH         hierarchy;
        TPMI_DH_OBJECT sequenceHandle;
        TPM2B_DATA     buffer;
} SequenceComplete_In;

typedef struct {
        TPMT_TK_HASHCHECK validation;
        TPM2B_DIGEST      result;
} SequenceComplete_Out;

TPM_RC TPM2_SequenceComplete(
        SequenceComplete_In    *in,               // IN: input parameter list
        SequenceComplete_Out   *out               // OUT: output parameter list
);

#endif // __TPM2_SEQUENCECOMPLETE_FP_H_
