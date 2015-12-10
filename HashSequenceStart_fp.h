/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_HASHSEQUENCESTART_FP_H
#define __TPM2_HASHSEQUENCESTART_FP_H

typedef struct {
        TPMI_ALG_HASH   hashAlg;
        TPM2B_AUTH      auth;
} HashSequenceStart_In;

typedef struct {
        TPMI_DH_OBJECT  sequenceHandle;
} HashSequenceStart_Out;

TPM_RC TPM2_HashSequenceStart(
        HashSequenceStart_In      *in,                   // IN: input parameter list
        HashSequenceStart_Out     *out                   // OUT: output parameter list
                              );

#endif // __TPM2_HASHSEQUENCESTART_FP_H
