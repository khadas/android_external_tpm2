/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_REWRAP_FP_H_
#define __TPM2_REWRAP_FP_H_

typedef struct {
        TPM2B_PRIVATE          inDuplicate;
        TPM_RH                 newParent;
        TPM_RH                 oldParent;
        TPM2B_ENCRYPTED_SECRET inSymSeed;
        TPM2B_NAME             name;
} Rewrap_In;

typedef struct {
        TPM2B_PRIVATE          outDuplicate;
        TPM2B_ENCRYPTED_SECRET outSymSeed;
} Rewrap_Out;

TPM_RC TPM2_Rewrap(
        Rewrap_In         *in,             // IN: input parameter list
        Rewrap_Out        *out             // OUT: output parameter list
);

#endif // __TPM2_REWRAP_FP_H_
