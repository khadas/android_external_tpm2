/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_EXTEND_FP_H
#define __TPM2_NV_EXTEND_FP_H

typedef struct {
        TPM_HANDLE  authHandle;
        TPM_HANDLE  nvIndex;
        TPM2B_DATA  data;
} NV_Extend_In;

TPM_RC TPM2_NV_Extend(
        NV_Extend_In      *in            // IN: input parameter list
);

#endif // __TPM2_NV_EXTEND_FP_H
