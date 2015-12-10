/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_STIRRANDOM_FP_H_
#define __TPM2_STIRRANDOM_FP_H_

typedef struct {
        TPM2B_DATA inData;
} StirRandom_In;

TPM_RC TPM2_StirRandom(
        StirRandom_In   *in            // IN: input parameter list
);

#endif // __TPM2_STIRRANDOM_FP_H_
