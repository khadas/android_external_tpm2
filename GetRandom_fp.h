/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_GETRANDOM_FP_H
#define __TPM2_GETRANDOM_FP_H

typedef struct {
        UINT32  bytesRequested;
} GetRandom_In;

typedef struct {
        TPM2B_DATA randomBytes;
} GetRandom_Out;

TPM_RC TPM2_GetRandom(
        GetRandom_In     *in,            // IN: input parameter list
        GetRandom_Out    *out            // OUT: output parameter list
);

#endif // __TPM2_GETRANDOM_FP_H
