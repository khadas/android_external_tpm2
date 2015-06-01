/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_NV_READPUBLIC_FP_H_
#define __TPM2_NV_READPUBLIC_FP_H_

typedef struct {
        TPMI_RH_NV_INDEX nvIndex;
} NV_ReadPublic_In;

typedef struct {
        TPM2B_NAME     nvName;
        TPMS_NV_PUBLIC nvPublic;
} NV_ReadPublic_Out;


TPM_RC TPM2_NV_ReadPublic(
        NV_ReadPublic_In      *in,          // IN: input parameter list
        NV_ReadPublic_Out     *out          // OUT: output parameter list
);

#endif // __TPM2_NV_READPUBLIC_FP_H_
