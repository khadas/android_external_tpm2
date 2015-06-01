/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_READ_FP_H
#define __TPM2_NV_READ_FP_H

typedef struct {
        TPMI_RH_NV_INDEX nvIndex;
        TPM_HANDLE       authHandle;
        UINT32           offset;
        UINT16           size;
} NV_Read_In;

typedef struct {
        TPM2B_DATA	data;
} NV_Read_Out;

TPM_RC TPM2_NV_Read(
        NV_Read_In        *in,                 // IN: input parameter list
        NV_Read_Out       *out                 // OUT: output parameter list
);

#endif // __TPM2_NV_READ_FP_H
