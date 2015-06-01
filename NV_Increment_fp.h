/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_INCREMENT_FP_H
#define __TPM2_NV_INCREMENT_FP_H

typedef struct {
        TPM_HANDLE       authHandle;
        TPMI_RH_NV_INDEX  nvIndex;
} NV_Increment_In;

TPM_RC TPM2_NV_Increment(
        NV_Increment_In       *in                  // IN: input parameter list
);

#endif // __TPM2_NV_INCREMENT_FP_H
