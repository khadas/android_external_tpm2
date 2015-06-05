/*
 * Copyright 2015 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef __TPM2_LOAD_FP_H
#define __TPM2_LOAD_FP_H

typedef struct {
  TPM2B_PRIVATE inPrivate;
  TPM2B_PUBLIC inPublic;
  TPM_HANDLE parentHandle;
} Load_In;

typedef struct {
  TPM2B_NAME name;
  TPMI_DH_OBJECT objectHandle;
} Load_Out;

TPM_RC TPM2_Load(Load_In *in,   // IN: input parameter list
                 Load_Out *out  // OUT: output parameter list
                 );

#endif  // __TPM2_LOAD_FP_H
