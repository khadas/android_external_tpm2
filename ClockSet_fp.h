/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_CLOCKSET_FP_H
#define __TPM2_CLOCKSET_FP_H

typedef struct {
        UINT64 newTime;
} ClockSet_In;

TPM_RC TPM2_ClockSet(
        ClockSet_In       *in              // IN: input parameter list
);

#endif // __TPM2_CLOCKSET_FP_H
