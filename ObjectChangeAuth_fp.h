/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_OBJECTCHANGEAUTH_FP_H_
#define __TPM2_OBJECTCHANGEAUTH_FP_H_

typedef struct {
        TPMI_DH_OBJECT  objectHandle;
        TPMI_DH_OBJECT  parentHandle;
        TPM2B_AUTH      newAuth;
} ObjectChangeAuth_In;

typedef struct {
        TPM2B_PRIVATE    outPrivate;
} ObjectChangeAuth_Out;

TPM_RC TPM2_ObjectChangeAuth(
        ObjectChangeAuth_In    *in,                // IN: input parameter list
        ObjectChangeAuth_Out   *out                // OUT: output parameter list
);

#endif // __TPM2_OBJECTCHANGEAUTH_FP_H_
