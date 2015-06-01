/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_WRITE_FP_H_
#define __TPM2_NV_WRITE_FP_H_

typedef struct {
        TPM_HANDLE        authHandle;
        TPMI_RH_NV_INDEX  nvIndex;
        TPM2B_DATA        data;
        UINT32            offset;
} NV_Write_In;

TPM_RC TPM2_NV_Write(
        NV_Write_In       *in                 // IN: input parameter list
);

#endif // __TPM2_NV_WRITE_FP_H_
