/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_POLICYNV_FP_H_
#define __TPM2_POLICYNV_FP_H_

typedef struct {
        TPM_HANDLE       policySession;
        TPMI_RH_NV_INDEX nvIndex;
        TPM_HANDLE       authHandle;
        UINT32           offset;
        TPM_EO           operation;
        TPM2B_DATA       operandB;
} PolicyNV_In;

TPM_RC TPM2_PolicyNV(
        PolicyNV_In       *in                  // IN: input parameter list
);

#endif // __TPM2_POLICYNV_FP_H_
