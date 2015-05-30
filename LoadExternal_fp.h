/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_LOADEXTERNAL_FP_H
#define __TPM2_LOADEXTERNAL_FP_H

typedef struct {
        TPMI_RH_HIERARCHY       hierarchy;
        TPM2B_PUBLIC            inPublic;
        TPM2B_SENSITIVE         inPrivate;
} LoadExternal_In;

typedef struct {
        TPM2B_NAME      name;
        TPMI_DH_OBJECT  objectHandle;
} LoadExternal_Out;

TPM_RC TPM2_LoadExternal(
        LoadExternal_In       *in,                   // IN: input parameter list
        LoadExternal_Out      *out                   // OUT: output parameter list
                         );

#endif // __TPM2_LOADEXTERNAL_FP_H
